template < typename ObjectType >
class FactoryPattern
{
   protected:
   bool m_debug;
   RingBufferWithLock<ObjectType*>* m_pool;

   public:
   FactoryPattern(const char* name, uint32_t pool_size=0, bool debug=false): 
       m_debug(debug),
       m_pool(0)
   {
      if(pool_size)
      {
        EnablePool(pool_size);
      }
   }

   uint64_t pool_size_bytes()
   {
       return (m_cur_pool_size * m_obj_size);
   }

   bool EnablePool(uint32_t max_size)
   {
      m_pool = new RingBufferWithLock<ObjectType*>(max_size);
      if(m_pool == 0) { return false; }
      m_rb_stats = &m_pool->stats();
      set_size(max_size);
      return true;
   }

   virtual void* GetMemory()
   {
      ObjectType*   pObj = 0;
      if(m_pool)
      {
        m_pool->remove(pObj);
      }
      if(pObj == 0)
      {
#ifdef MOVIK_MEMORY_DEBUG
          pObj = (ObjectType*)::operator new(sizeof(ObjectType),__FILE__,__LINE__);
#else
          pObj = (ObjectType*)::operator new(sizeof(ObjectType));
#endif
      }
      return (void*)pObj;
   }

   ObjectType* Create()
   {
      ObjectType* pObj = new (GetMemory()) ObjectType;
      if(pObj) 
      {
          __sync_fetch_and_add(&m_obj_created, 1);
          __sync_fetch_and_add(&m_num_objects, 1);
          if(m_num_objects > m_high_mark)
          {
             m_high_mark = m_num_objects;
          }
      }
      /*Since GetMemory is virtual function, if some other class is using
       * different method to get memory we need more stats like failed to get
       * memory.. etc */
      else //if(!pObj)
      {
        __sync_fetch_and_add(&m_outOfMemoryAttempts, 1);
      }
      return pObj;
   }

   template <typename A1>
   ObjectType* Create(A1 a1)
   {
      ObjectType* pObj = new (GetMemory()) ObjectType(a1);
      if(pObj) 
      {
          __sync_fetch_and_add(&m_obj_created, 1);
          __sync_fetch_and_add(&m_num_objects, 1);
          if(m_num_objects > m_high_mark)
          {
             m_high_mark = m_num_objects;
          }
      }
      else //if(!pObj)
      {
        __sync_fetch_and_add(&m_outOfMemoryAttempts, 1);
      }
      return pObj;
   }

   virtual void Delete(ObjectType* pObj)
   {
      if(m_debug)
      {
       printf("Factory<%s>: deleting %p\n", m_name, pObj);
       fflush(stdout);
      }
      if(pObj == 0)
      {
        return;
      }
      pObj->~ObjectType(); 
      if(m_pool)
      {
         if(!m_pool->insert(pObj)) 
         { 
            ::operator delete((void*)pObj);
         }
      }else
      {
          ::operator delete((void*)pObj);
      }

      __sync_fetch_and_add(&m_obj_deleted, 1);

      uint64_t newValue = __sync_sub_and_fetch(&m_num_objects, 1);
      assert (newValue != (uint64_t)(-1));
   }
   virtual ~FactoryPattern() {};
};

template < typename ObjectType >
class FactoryPatternStack : public FactoryPattern<ObjectType>
{
private:
    StackAllocator<ObjectType> m_stackAllocator;
    //FactoryPatternStack(const FactoryPatternStack<ObjectType> &rhs);
public:
    FactoryPatternStack(const char* name, 
                        uint32_t    initialSize=0, 
                        uint32_t    growthSlab=1, 
                        uint32_t    maxSize=1,
                        uint32_t    chunkSize=1,
                        bool        debug=false): 
        FactoryPattern<ObjectType>(name),
        m_stackAllocator(initialSize, 
                         growthSlab,
                         maxSize, chunkSize)
    {
        FactoryPattern<ObjectType>::set_size(initialSize);
    }

    void* GetMemory()
    {
      FactoryPattern<ObjectType>::set_size(m_stackAllocator.size());
      return (void *)m_stackAllocator.getObject();
    }

    void Delete(ObjectType* pObj)
    {
      if(pObj == 0)
      {
        return;
      }
#ifdef DBG_VALGRIND
      delete(pObj); 
#else
      pObj->~ObjectType(); 
      m_stackAllocator.freeObject(pObj); 
#endif

      __sync_fetch_and_add(&(this->m_obj_deleted), 1);

       uint64_t newValue = __sync_sub_and_fetch(&(this->m_num_objects), 1);

      assert (newValue != (uint64_t)(-1));

    }
    ~FactoryPatternStack();

};

class RanapPdpSession
{
    static const uint32_t sm_nmPoolInitialSize    = 30000;
    static const uint32_t sm_nmPoolGrowthSlab     = 10000;
    static const uint32_t sm_nmPoolMaxSizeChassis = 2000000;
    static const uint32_t sm_nmPoolMaxSizeTelco   = 400000;
    static const uint32_t sm_nmPoolChunkSize      = 16;

public:
    ~RanapPdpSession();

    RanapPdpSession();

    static RanapPdpSession* Create();

    void DeleteThis();

private:
    static FactoryPatternStack<RanapPdpSession> factory;
    //friend class FactoryPatternStack<RanapPdpSession>;
    RanapPdpSession();
};

FactoryPatternStack<RanapPdpSession> RanapPdpSession::factory = factoryInit<RanapPdpSession>("RanapPdpSession",
                                                                                             RanapPdpSession::sm_nmPoolInitialSize, 
                                                                                             RanapPdpSession::sm_nmPoolGrowthSlab,
                                                                                             RanapPdpSession::sm_nmPoolMaxSizeChassis,
                                                                                             RanapPdpSession::sm_nmPoolChunkSize,
 
                                                                                             false);
RanapPdpSession* RanapPdpSession::Create()
{
    RanapPdpSession* pPkt = factory.Create();
    return pPkt;
}
