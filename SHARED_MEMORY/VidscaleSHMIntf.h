#ifndef _VIDSCALE_SHM_INTF_H_
#define _VIDSCALE_SHM_INTF_H_

#include "SHMStructures.h"
#include "pthread.h"
#include "iostream"
#define SHM_INFO_SIZE sizeof(ShmInfo)


class VidscaleSHMIntf
{
public:
    VidscaleSHMIntf();
    ~VidscaleSHMIntf();
    static VidscaleSHMIntf& getInstance()
    {
      static VidscaleSHMIntf _singleton;
      return _singleton;
    }

    umtsRab* getSHMSession(char* key, ShmKeyType keyType);
    umtsRab* getFirstSHMSession(ShmKeyType keyType, Hash_Search *searchPtr);
    umtsRab* getNextSHMSession(Hash_Search *searchPtr, ShmKeyType keyType);

    umtsRab* createSHMHashEntry(char* key, ShmKeyType keyType, umtsRab* pSHMRab, umtsRab *pOldRabRec);
    bool deleteSHMHashEntry(char* key, ShmKeyType keyType, uint64_t imsi = 0);

    const char *getSessionShmGlobalKey() {return m_sessionShmGlobalKey.c_str();}
    void setSessionShmGlobalKey(const char *shmKey) {m_sessionShmGlobalKey.assign(shmKey);}
    void initHashTable(SHM_Hash_Table* pHashTable);
    void Init();

    uint32_t getSessionShmSize() {return m_sessionShmSize;}
    void *getSharedBase() { return m_sharedBase; }
    //  void printErrSessionInfo(umtsRab *shmSess);
    //  void printInfoSessionInfo(umtsRab *shmSess);

    uint32_t getSessionPoolFreeQueueSize() {return phashValueFreeMemMgrObj->freeQueueSize();}
    uint32_t getHashEntryPoolFreeQueueSize() {return phashEntryFreeMemMgrObj->freeQueueSize();}

    uint32_t getSessionPoolMaxQueueSize() {return phashValueFreeMemMgrObj->maxQueueSize();}
    uint32_t getHashEntryPoolMaxQueueSize() {return phashEntryFreeMemMgrObj->maxQueueSize();}

    uint32_t getHashTablePoolFreeQueueSize() {return phashTableFreeMemMgrObj->freeQueueSize();}
    uint32_t getHashTablePoolMaxQueueSize() {return phashTableFreeMemMgrObj->maxQueueSize();}

    void Hash_InitTable(SHM_Hash_Table *t, FreeMemMgr<SHM_Hash_Entry> *phashEntryFreeMemMgr,
                        FreeMemMgr<umtsRab> *psessionFreeMemMgr,
                        void *pSharedMemBase, ShmKeyType keyType = SHM_KEY_INVALID);

    umtsRab *Hash_FindEntry(const SHM_Hash_Table *,
                            const char * key,
                            unsigned int length);

    bool Hash_CreateEntry(SHM_Hash_Table * pTable,
                          const char *key,
                          unsigned int keyLength,
                          int valueOffset,
                          uint8_t partitionIndex,
                          pthread_mutex_t *shmUpdateLock = NULL,
                          int *oldSessValueOffset = NULL);

    umtsRab* Hash_DeleteEntry(SHM_Hash_Table *t,
                              const char *key,
                              unsigned int length,
                              uint8_t partitionIndex,
                              pthread_mutex_t *shmUpdateLock = NULL
                             );

    bool Hash_EnumFirst(SHM_Hash_Table *, Hash_Search *);
    bool Hash_EnumNext(Hash_Search *);
    int  Hash_getNumEntries(SHM_Hash_Table *t);
    void Hash_DeleteTable(SHM_Hash_Table *t);

    bool createRabRecordInSharedMemory(umtsRab *pNewRab);
    bool removeRabRecordInSharedMemory(umtsRab *pRabObject);

    FreeMemMgr<umtsRab> * getHashValueMemMgrObj() {return phashValueFreeMemMgrObj;}

private:
    void divideAndInitSharedMemory();

    FreeMemMgr<umtsRab>             *phashValueFreeMemMgrObj; /* Session pool manager */
    FreeMemMgr<SHM_Hash_Table>      *phashTableFreeMemMgrObj; /* Pool of hash tables */
    FreeMemMgr<SHM_Hash_Entry>      *phashEntryFreeMemMgrObj; /* Pool of hash key, value pair objects */
    void                            *m_sharedBase;
    std::string                      m_sessionShmGlobalKey;
    uint32_t                         m_numOfRabObjs;
    uint32_t                         m_rabObjsSize;
    uint32_t                         m_sessionShmSize;
    uint32_t                         m_hashObjsOffset;
    uint32_t                         m_hashObjsSize;
    uint32_t                         m_numHashObjs;
    pthread_mutex_t                  m_shmUpdateLock;
    uint32_t                         sm_firstTunnelStrideOffset;
    uint32_t                         sm_secondStrideOffset;
    uint32_t                         sm_firstTunnelStrideSize; 
    uint32_t                         sm_numSecondStride;
    uint32_t                         sm_secondStrideSize;
    uint32_t                         sm_rabObjOffset;
};

#endif
