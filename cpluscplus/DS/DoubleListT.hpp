//cae/main/dev/src/common/ds/DoubleListT.hpp#10 - edit change 5149 (xtext)
#ifndef __DOUBLE_LIST_T_H__
#define __DOUBLE_LIST_T_H__

template
<
  typename ContentType
>
class DoubleListT
{
    public:

    DoubleListT(bool autoClean=false);

    ~DoubleListT();

    ContentType* First() { return m_pHead; }
    
    ContentType* Last() { return m_pTail; }

    ContentType* Next(ContentType* pNode);

    ContentType* Prev(ContentType* pNode);

    bool AddHead(ContentType* pNode);

    bool AddTail(ContentType* pNode);

    bool AddSorted(ContentType* pNode);

    bool AddBefore(ContentType* pRefNode, ContentType* pNode);

    ContentType* RemoveFirst();

    bool RemoveNode(ContentType* pNode);

    void Iterate(void(*callback)(ContentType* pItem, void*), void* ptr);

    void IterateConditional(bool(*callback)(ContentType* pItem, void*), void* ptr);

    bool IterateConditionalFromLastStored(bool(*callback)(ContentType* pItem, void*), void* ptr);

    void Iterate(void(*callback)(DoubleListT<ContentType>& rList, ContentType* pItem, void*), void* ptr);

    void IterateReverse(void(*callback)(ContentType* pItem, void*), void* ptr);

    uint64_t long Size() { return m_insert - m_delete; }
    
    void MakeEmpty();

    void Dump();

    protected:

    ContentType* m_pHead;
    ContentType* m_pTail;
    ContentType* m_pNextNodeToBeTraversed;
    uint64_t m_insert;
    uint64_t m_delete;
    bool m_bAutoClean;
};

template
<
  typename ContentType
>
class DoubleListNode
{
  ContentType* m_pPrev;
  ContentType* m_pNext;
  public:
  DoubleListNode() { m_pNext = 0; m_pPrev=0; }
  ContentType* Prev() { return m_pPrev; }
  ContentType* Next() { return m_pNext; }
  void SetPrev(ContentType* pPrev) { m_pPrev = pPrev; }
  void SetNext(ContentType* pNext) { m_pNext = pNext; }
};

template<typename ContentType>
DoubleListT<ContentType>::DoubleListT(bool autoClean) :
   m_pHead(0),
   m_pTail(0), 
   m_pNextNodeToBeTraversed(0), 
   m_insert(0),
   m_delete(0),
   m_bAutoClean(autoClean)
{ 
}

template<typename ContentType>
DoubleListT<ContentType>::~DoubleListT()
{
    ContentType* pCurrent = m_pHead;
    if(m_bAutoClean)
    {
      while(pCurrent)
      {
         ContentType* pNext = (ContentType*)(pCurrent->Next());
         pCurrent->DeleteSelf();
         pCurrent = pNext;
      }
   }
   m_pHead = m_pTail = 0;
   m_insert = 0; 
   m_delete = 0; 
}

template<typename ContentType>
ContentType* DoubleListT<ContentType>::Next(ContentType* pNode)
{
    if(pNode)
    {
       return pNode->Next();
    }
    return 0;
}

template<typename ContentType>
ContentType* DoubleListT<ContentType>::Prev(ContentType* pNode)
{
    if(pNode)
    {
       return pNode->Prev();
    }
    return 0;
}

template<typename ContentType>
bool DoubleListT<ContentType>::AddHead(ContentType* pNode)
{
   if((pNode == 0) || pNode->Next() || pNode->Prev())
   {
      return false;
   }

   m_insert++;

   if(m_pHead == 0)
   {
      m_pHead = m_pTail = pNode;
      return true;
   }

   m_pHead->SetPrev(pNode);
   pNode->SetNext(m_pHead);
   m_pHead = pNode;
   return true;
}

template<typename ContentType>
bool DoubleListT<ContentType>::AddBefore(ContentType* pCurrent, ContentType* pNode)
{
   if(pCurrent == m_pHead)
   {
      return AddHead(pNode);
   }

   if(pCurrent == 0)
   {
      return AddTail(pNode);
   }

   pNode->SetPrev(pCurrent->Prev());
   pNode->SetNext(pCurrent);

   if(pCurrent->Prev())
   {
      pCurrent->Prev()->SetNext(pNode);
   }
   pCurrent->SetPrev(pNode);

   m_insert++;
   return true;
}

template<typename ContentType>
bool DoubleListT<ContentType>::AddTail(ContentType* pNode)
{
   if((pNode == 0) || pNode->Next() || pNode->Prev())
   {
      return false;
   }

   m_insert++;

   if(m_pHead == 0)
   {
      m_pHead = m_pTail = pNode;
      return true;
   }

   m_pTail->SetNext(pNode);
   pNode->SetPrev(m_pTail);
   m_pTail = pNode;
   return true;
}

template<typename ContentType>
ContentType* DoubleListT<ContentType>::RemoveFirst()
{
    if(!m_pHead) { return 0; }
    ContentType* head = m_pHead;
    RemoveNode(m_pHead);
    return head;
}

template<typename ContentType>
bool DoubleListT<ContentType>::RemoveNode(ContentType* pNode)
{
    if(pNode == 0) 
    {
        return false;
    }

    m_delete++;

    if((pNode == m_pHead) && (pNode == m_pTail))
    {
       m_pHead = m_pTail = 0;
       pNode->SetNext(0);
       pNode->SetPrev(0);
       return true;
    }

    if(pNode == m_pHead)
    {
       m_pHead = m_pHead->Next();
       m_pHead->SetPrev(0);
       pNode->SetNext(0);
       pNode->SetPrev(0);
       return true;
    }

    if(pNode == m_pTail)
    {
       m_pTail = m_pTail->Prev();
       m_pTail->SetNext(0);
       pNode->SetNext(0);
       pNode->SetPrev(0);
       return true;
    }

    ContentType* pPrev = pNode->Prev();
    ContentType* pNext = pNode->Next();

    pPrev->SetNext(pNext);
    pNext->SetPrev(pPrev);

    pNode->SetNext(0);
    pNode->SetPrev(0);
    return true;
}

template<typename ContentType>
void DoubleListT<ContentType>::Iterate(void(*callback)(ContentType* pItem, void*), void* ptr)
{
    ContentType* pCurrent = m_pHead;
    while(pCurrent)
    {
       ContentType* pNext = (ContentType*)(pCurrent->Next());
       callback(pCurrent, ptr);
       pCurrent = pNext;
    }
}

template<typename ContentType>
void DoubleListT<ContentType>::IterateConditional(bool(*callback)(ContentType* pItem, void*), void* ptr)
{
    ContentType* pCurrent = m_pHead;
    bool stop_iteration = false;
    while(pCurrent)
    {
       ContentType* pNext = pCurrent->Next();
       stop_iteration = callback(pCurrent, ptr);
       if (stop_iteration)
       {
         break;
       }
       pCurrent = pNext;
    }
}

// NOTE : This function call works only for those implemetations where there is no deletion of the element 
// happens only addition to the list. The reason is we store the nextNodeToBeTraversed and in the next
// iteration start from that, There is a real possibility of this node being deleted and then the access
// to it would result in segmentation fault.
template<typename ContentType>
bool DoubleListT<ContentType>::IterateConditionalFromLastStored(bool(*callback)(ContentType* pItem, void*), void* ptr)
{
    // start traversing from head or the last stored location
    ContentType* pCurrent = m_pHead;
    if (m_pNextNodeToBeTraversed)
    {
      pCurrent = m_pNextNodeToBeTraversed;
    }

    bool halt_iteration = false;
    while(pCurrent)
    {
      ContentType* pNext = (ContentType*) (pCurrent->Next());
      halt_iteration = callback(pCurrent, ptr);
      pCurrent = pNext;

      if (halt_iteration)
      {
        if (!pCurrent)
        {
          // The List is traversed completely and halt_teration is also set to be true,
          // set the halt_iteration to false to avoid infinite loop in app.
          halt_iteration = false;
        }
        break;
      }
    }

    if (halt_iteration)
    {
      m_pNextNodeToBeTraversed = pCurrent;
    }
    else
    {
      m_pNextNodeToBeTraversed = 0;
    }

    return halt_iteration;
}

template<typename ContentType>
void DoubleListT<ContentType>::Iterate(void(*callback)(DoubleListT<ContentType>& rList, ContentType* pItem, void*), void* ptr)
{
    ContentType* pCurrent = m_pHead;
    while(pCurrent)
    {
       ContentType* pNext = pCurrent->Next();
       callback(*this, pCurrent, ptr);
       pCurrent = pNext;
    }
}

template<typename ContentType>
void DoubleListT<ContentType>::IterateReverse(void(*callback)(ContentType* pItem, void*), void* ptr)
{
    ContentType* pCurrent = m_pTail;
    while(pCurrent)
    {
       ContentType* pPrev = pCurrent->Prev();
       callback(pCurrent, ptr);
       pCurrent = pPrev;
    }
}

template<typename ContentType>
bool DoubleListT<ContentType>::AddSorted(ContentType* pNode)
{
   if(m_pHead == 0)
   {
      return AddHead(pNode);
   }

   ContentType* pCurrent = this->m_pHead;
   while(pCurrent)
   {
      if(pCurrent->Compare(*pNode) > 0)
      {
         return AddBefore(pCurrent, pNode);
      }
      pCurrent = (ContentType*)(pCurrent->Next());
   }

   return AddTail(pNode);
}

/*
 * NOTE: AVOID THE USE OF THIS FUNCTION
 */
template <typename ContentType>
void DoubleListT<ContentType>::MakeEmpty()
{
    ContentType* pCurrent = m_pHead;
    if(m_bAutoClean)
    {
      while(pCurrent)
      {
         ContentType* pNext = pCurrent->Next();
         pCurrent->DeleteSelf();
         pCurrent = pNext;
      }
   }
   m_pHead = 0;
   m_pTail = 0;
   m_insert = 0; 
   m_delete = 0; 
}

template <typename ContentType>
void DoubleListT<ContentType>::Dump()
{
    ContentType* pCurrent = m_pHead;
    while(pCurrent)
    {
      pCurrent->Dump();
      pCurrent = pCurrent->Next();
    }
}

#endif
