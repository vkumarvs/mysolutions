/*-
 * Copyright (c) 1988, 1989, 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1988, 1989 by Adam de Boor
 * Copyright (c) 1989 by Berkeley Softworks
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Adam de Boor.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @(#)hash.c	8.1 (Berkeley) 6/6/93
 */

#include "assert.h"
#include "VidscaleSHMIntf.h"
#include "jenkins3_hash.h"
#include <string.h>
#include "AutoLock.h"

#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>


#define MAX_SHM_KEYS 1
//const char *sharedMemKey[MAX_SHM_KEYS]={"/sharedMemKey1","/sharedMemKey2"};
const char *sharedMemKey[MAX_SHM_KEYS]={"/sharedMemKey2"};
static uint32_t (*jhash)(const void *key, uint32_t len, uint32_t initval) = jenkins3_jhash;

uint64_t perfarray[500]={0}; //XXX Remove: will be used to count the termination of "find" loop.

uint32_t ipHash(const struct Ipv4v6storage *ip)
{
	uint32_t hashVal = 0;
	if (ip->ss_family == AF_INET6)
	{
		hashVal = (((ip->__data[6] << 8) | ip->__data[7]) ^
				((ip->__data[8] << 8) | ip->__data[9]) ^
				((ip->__data[10] << 8) | ip->__data[11]) ^
				((ip->__data[12] << 8) | ip->__data[13]) ^
				((ip->__data[14] << 8) | ip->__data[15]));
	}
	else
	{
		// assume IPv4
		hashVal = ((ip->__data[2] << 8) | ip->__data[3]);
	}

	return hashVal;
}

/* hash.c --
 *
 * 	This module contains routines to manipulate a hash table.
 * 	See hash.h for a definition of the structure of the hash
 * 	table.  Hash tables grow automatically as the amount of
 * 	information increases.
 */



/*
 *---------------------------------------------------------
 *
 * Hash_InitTable --
 *
 * 	Set up the hash table t with a given number of buckets, or a
 * 	reasonable default if the number requested is less than or
 * 	equal to zero.  Hash tables will grow in size as needed.
 *
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Memory is allocated for the initial bucket area.
 *
 *---------------------------------------------------------
 */
void VidscaleSHMIntf::
Hash_InitTable(SHM_Hash_Table *t, FreeMemMgr<SHM_Hash_Entry>
               *phashEntryFreeMemMgr, FreeMemMgr<umtsRab>
               *psessionFreeMemMgr, void *pSharedMemBase, ShmKeyType keyType)
{

    for(int index=0; index < HASH_TABLE_SIZE; index++)
    {
        t->offsetHashEntry[index] = 0;
    }

  //  t->m_HashTableKeyType = keyType; 
    t->phashEntryFreeMemMgrObj = (void*)phashEntryFreeMemMgr;
    t->phashValueFreeMemMgrObj = (void*)psessionFreeMemMgr;

    t->m_sharedBase = pSharedMemBase;
    memset(&(t->tableStats), 0, sizeof(Hash_Table_Stats));
}

#if 0
/*
 *---------------------------------------------------------
 *
 * Hash_DeleteTable --
 *
 *	This routine removes everything from a hash table
 *	and frees up the memory space it occupied (except for
 *	the space in the SHM_Hash_Table structure).
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Lots of memory is freed up.
 *
 *---------------------------------------------------------
 */
void VidscaleSHMIntf::
Hash_DeleteTable(SHM_Hash_Table *t)
{
	/*
	 * Set up the hash table to cause memory faults on any future access
	 * attempts until re-initialization.
	 */
    for(int index = 0; index < HASH_TABLE_SIZE; index++)
    {
        int nextHashEntryOffset = t->offsetHashEntry[index];
        while(0 != nextHashEntryOffset)
        {
            SHM_Hash_Entry *pCurrHashObj = (SHM_Hash_Entry *)(offToPtrHash(t, nextHashEntryOffset));
            int currHashEntryOff = nextHashEntryOffset;
            nextHashEntryOffset = pCurrHashObj->offsetNext;
            ((FreeMemMgr<SHM_Hash_Entry>
              *)t->phashEntryFreeMemMgrObj)->pushFreeMem((SHM_Hash_Entry*)(offToPtrHash(t,
                                                                                        currHashEntryOff)));
            ((FreeMemMgr<umtsRab> *)t->phashValueFreeMemMgrObj)->pushFreeMem((umtsRab*)offToPtrHash(t, pCurrHashObj->offsetValue));
        }
        t->offsetHashEntry[index] = 0;
    }

}
#endif

/*
 *---------------------------------------------------------
 *
 * Hash_FindEntry --
 *
 * 	Searches a hash table for an entry corresponding to key.
 *
 * Results:
 *	The return value is a pointer to the entry for key,
 *	if key was present in the table.  If key was not
 *	present, NULL is returned.
 *
 * Side Effects:
 *	None.
 *
 *---------------------------------------------------------
 */
umtsRab * VidscaleSHMIntf::
Hash_FindEntry(const SHM_Hash_Table *t, const char *key, unsigned int length)
{
	unsigned int h;
        const char *p;
        unsigned int i;
        LogDebug("Hash_FindEntry\n");

#ifdef ORIG
        for (h = 0, p = key; *p;)
            h = (h << 5) - h + *p++;
#else

        for (h = 0, p = key, i=0; i < length;)
        {
            h = (h << 5) - h + *p++;
            i++;
        }

#endif
        LogDebug("Hash_FindEntry hash %u\n", h);
	p = key;
        int nextHashEntryOffset = t->offsetHashEntry[h % HASH_TABLE_SIZE];
        for (;(0 != nextHashEntryOffset);)
        {
            SHM_Hash_Entry *pCurrHashObj = (SHM_Hash_Entry *)(offToPtrHash(t, nextHashEntryOffset));
            if(!pCurrHashObj)
            {
              LogDebug("Hash_FindEntry entry not found\n");
              return NULL;
            }
            if (memcmp(&(pCurrHashObj->hashKey), key, length) == 0)
            {
                LogDebug("Hash_FindEntry found the entry offset %Zu \n", ptrToOffHash(t, pCurrHashObj));
                return ((umtsRab*)offToPtrHash(t, pCurrHashObj->offsetValue));
            }
            nextHashEntryOffset = pCurrHashObj->offsetNext;

        }

        LogDebug("Hash_FindEntry not found entry\n");
        return (NULL);
}

/*
 *---------------------------------------------------------
 *
 * Hash_CreateEntry --
 *
 *	Searches a hash table for an entry corresponding to
 *	key.  If no entry is found, then one is created.
 *
 * Results:
 *	The return value is a pointer to the entry.  If *newPtr
 *	isn't NULL, then *newPtr is filled in with TRUE if a
 *	new entry was created, and FALSE if an entry already existed
 *	with the given key.
 *
 * Side Effects:
 *	Memory may be allocated, and the hash buckets may be modified.
 *---------------------------------------------------------
 */
bool VidscaleSHMIntf::
Hash_CreateEntry(SHM_Hash_Table *t, const char *key,unsigned int length, int
                 valueOffset, uint8_t partitionIndex, pthread_mutex_t *shmUpdateLock, int *oldSessValueOffset)
{
    unsigned int h;
    LogDebug("Hash_CreateEntry\n");
    if(!shmUpdateLock)
    {
      LogError("SHM UPDATE LOCk IS NULL\n");
      return FALSE;	
    }

    h = jhash(key, length, 10);
    h = h & (HASH_TABLE_SIZE - 1);

    /*First Look if particular key exist*/

    LogDebug("Hash_CreateEntry hash index %u \n", h);

    SHM_Hash_Entry *pNewHashObj = NULL;
    if(((FreeMemMgr<SHM_Hash_Entry>*)t->phashEntryFreeMemMgrObj))
    {

      pNewHashObj = ((FreeMemMgr<SHM_Hash_Entry>
                      *)t->phashEntryFreeMemMgrObj)->getFreeMem(partitionIndex);
    }
    else
    {
      LogError("Invalid pointer for NewHashObject Memory Manager : HashTable Pointer %p \n", t);
      return FALSE;
    }
    if(NULL == pNewHashObj)
    {
      LogError("Not enough memory for hash entry \n");
      return FALSE;
    }

    pNewHashObj->offsetValue = valueOffset;
    pNewHashObj->keySize = length;
    memcpy(&(pNewHashObj->hashKey), key, length);
    pNewHashObj->offsetNext = 0;

    int loopcount = 0;
    {
      AutoLock lock(*shmUpdateLock);
      if(0 == t->offsetHashEntry[h]) //first entry of the bucket
      {
        (perfarray[0])++;
        t->offsetHashEntry[h] = ptrToOffHash(t, pNewHashObj);
        return TRUE;
      }

      bool isLastElemRemoved = false;
      SHM_Hash_Entry *pCurrHashObj = NULL;
      SHM_Hash_Entry *pTmpLastHashObj = NULL;
      int nextHashEntryOffset = t->offsetHashEntry[h];
      int currentHashEntryOffset = nextHashEntryOffset;

      for (;(0 != nextHashEntryOffset);)
      {
        loopcount++; //For benchmarking has debug support.
        __sync_fetch_and_add(&(perfarray[loopcount]) , 1);
        pCurrHashObj = (SHM_Hash_Entry *)(offToPtrHash(t, nextHashEntryOffset));
        if(!pCurrHashObj)
        {
          LogInfo("Hash_CreateEntry hash entry not found offset hashbucket %u tableptr %p\n", h, t );
          break;
        }
        if (memcmp(&(pCurrHashObj->hashKey), key, length) == 0)
        {
          int16_t *node;
          node = (int16_t*)&valueOffset;

          if(currentHashEntryOffset == nextHashEntryOffset) //remove the head element
          {
            LogInfo("Hash_CreateEntry hash entry found. Head element offSet %Zu hashbucket %u tableptr %p\n", ptrToOffHash(t, pCurrHashObj), h, t);
            t->offsetHashEntry[h] = pCurrHashObj->offsetNext;
          }
          else //remove non head element
          {
            LogInfo("Hash_CreateEntry hash entry found. Non head element offSet %Zu hashbucket %u tableptr %p\n", ptrToOffHash(t, pCurrHashObj), h, t);
            ((SHM_Hash_Entry *)(offToPtrHash(t, currentHashEntryOffset)))->offsetNext = pCurrHashObj->offsetNext;
          }

          if(oldSessValueOffset)
          {
            *oldSessValueOffset  = pCurrHashObj->offsetValue;
          }

          if(pCurrHashObj->offsetNext == 0) //If it is last element
          {
            isLastElemRemoved  = true;
            pTmpLastHashObj = (SHM_Hash_Entry *)(offToPtrHash(t, currentHashEntryOffset));
            LogInfo("Last element removed from list \n");
          }

          LogInfo("pushFreeMem SHM_Hash_Entry %p, pCurrHashObj %p m_sharedBase %p\n", offToPtrHash(t, currentHashEntryOffset), pCurrHashObj, t->m_sharedBase);
          ((FreeMemMgr<SHM_Hash_Entry>
            *)t->phashEntryFreeMemMgrObj)->pushFreeMem(pCurrHashObj, partitionIndex);
        }
        currentHashEntryOffset = nextHashEntryOffset;
        nextHashEntryOffset = pCurrHashObj->offsetNext;
      }

      (perfarray[loopcount])++;
      LogDebug("Inserting entry at the end of the chain\n");
      if(isLastElemRemoved)
      {
        pTmpLastHashObj->offsetNext = ptrToOffHash(t, pNewHashObj);
      }
      else if(pCurrHashObj)
      {
        pCurrHashObj->offsetNext = ptrToOffHash(t, pNewHashObj);
      }
      else
      {
        LogError("HashIndex (%u) pCurrHashObj entry is NULL-- startHashEntryOffset %u\n", h, t->offsetHashEntry[h]);
        return FALSE;
      }
      LogInfo("Hash Entry created at the end of the chain ptr %p offSet %Zu hashbucket %u tableptr %p\n\n\n", pNewHashObj, ptrToOffHash(t, pNewHashObj), h, t);
    }

    return TRUE;
}

/*
 *---------------------------------------------------------
 *
 * Hash_DeleteEntry --
 *
 * 	Delete the given hash table entry and free memory associated with
 *	it.
 *
 * Results:
 *	None.
 *
 * Side Effects:
 *	Hash chain that entry lives in is modified and memory is freed.
 *
 *---------------------------------------------------------
 */
umtsRab* VidscaleSHMIntf::
Hash_DeleteEntry(SHM_Hash_Table *t, const char *key,unsigned int length, uint8_t partitionIndex,
                  pthread_mutex_t *shmUpdateLock)
{
    unsigned int h;
    LogDebug("Hash_DeleteEntry\n");
    if(!shmUpdateLock)
    {
      LogError("SHM UPDATE LOCk IS NULL\n");
      return FALSE;	
    }

    h = jhash(key, length, 10);
    h = h & (HASH_TABLE_SIZE - 1);
    LogDebug("Hash_DeleteEntry hash %u\n", h);


      AutoLock lock(*shmUpdateLock);

    int nextHashEntryOffset = t->offsetHashEntry[h];
    int currentHashEntryOffset = nextHashEntryOffset;
    for (;(0 != nextHashEntryOffset);)
    {
      SHM_Hash_Entry *pNextHashObj = (SHM_Hash_Entry *)(offToPtrHash(t, nextHashEntryOffset));
      if(!pNextHashObj)
      {
        LogInfo("Hash_DeleteEntry hash entry not found offset hashbucket %u tableptr %p\n", h, t );
        return NULL;
      }
      if (memcmp(&(pNextHashObj->hashKey), key, length) == 0)
      {
        if(currentHashEntryOffset == nextHashEntryOffset) //remove the head element
        {
          LogInfo("Hash_DeleteEntry hash entry found. Head element offSet %Zu hashbucket %u tableptr %p\n", ptrToOffHash(t, pNextHashObj), h, t);
          t->offsetHashEntry[h] = pNextHashObj->offsetNext;
        }
        else //remove non head element
        {
          LogInfo("Hash_DeleteEntry hash entry found. Non head element offSet %Zu hashbucket %u tableptr %p\n", ptrToOffHash(t, pNextHashObj), h, t);
          ((SHM_Hash_Entry *)(offToPtrHash(t, currentHashEntryOffset)))->offsetNext = pNextHashObj->offsetNext;
        }
        int sessOffset = pNextHashObj->offsetValue;
        umtsRab* pRabRec =  (umtsRab*)offToPtrHash(t, sessOffset);
        LogDebug("pushFreeMem SHM_Hash_Entry %p, pNextHashObj %p m_sharedBase %p\n", offToPtrHash(t, currentHashEntryOffset), pNextHashObj, t->m_sharedBase);
    if(((FreeMemMgr<SHM_Hash_Entry>*)t->phashEntryFreeMemMgrObj))
    {
        ((FreeMemMgr<SHM_Hash_Entry>
          *)t->phashEntryFreeMemMgrObj)->pushFreeMem(pNextHashObj,
                                                     partitionIndex);
    }
    else
    {
      LogError("Invalid HashEntry Object Memory Manager pointer inside hashTable (%p) \n", t);
      return NULL;
    }
        return pRabRec;
      }
      currentHashEntryOffset = nextHashEntryOffset;
      nextHashEntryOffset = pNextHashObj->offsetNext;
    }
    LogInfo("Hash_DeleteEntry hash entry not found end offset hashbucket %u tableptr %p\n", h, t );
    return NULL;
}

/*
 *---------------------------------------------------------
 *
 * Hash_EnumFirst --
 *	This procedure sets things up for a complete search
 *	of all entries recorded in the hash table.
 *
 * Results:
 *	The return value is the address of the first entry in
 *	the hash table, or NULL if the table is empty.
 *
 * Side Effects:
 *	The information in searchPtr is initialized so that successive
 *	calls to Hash_Next will return successive HashEntry's
 *	from the table.
 *
 *---------------------------------------------------------
 */
bool VidscaleSHMIntf::
Hash_EnumFirst(SHM_Hash_Table *t, Hash_Search *searchPtr)
{

	(searchPtr->tablePtr) = t;
	(searchPtr->nextIndex) = 0;
	(searchPtr->nextOffsetHashEntry) = t->offsetHashEntry[0];
#if 0
        while(0 == (searchPtr->nextOffsetHashEntry))
        {
            (*(searchPtr->nextIndex))++;
	    *(searchPtr->nextOffsetHashEntry) = t->offsetHashEntry[*(searchPtr->nextIndex)];
        }
#endif
	return (Hash_EnumNext(searchPtr));
}

/*
 *---------------------------------------------------------
 *
 * Hash_EnumNext --
 *    This procedure returns successive entries in the hash table.
 *
 * Results:
 *    The return value is a pointer to the next HashEntry
 *    in the table, or NULL when the end of the table is
 *    reached.
 *
 * Side Effects:
 *    The information in searchPtr is modified to advance to the
 *    next entry.
 *
 *---------------------------------------------------------
 */
bool VidscaleSHMIntf::Hash_EnumNext(Hash_Search *searchPtr)
{
	const SHM_Hash_Table *t = searchPtr->tablePtr;

	/*
	 * The hashEntryPtr field points to the most recently returned
	 * entry, or is NULL if we are starting up.  If not NULL, we have
	 * to start at the next one in the chain.
	 */
        if(((searchPtr->nextIndex) == HASH_TABLE_SIZE) && (0 == (searchPtr->nextOffsetHashEntry)))
        {
            return FALSE;
        }
        if(0 == (searchPtr->nextOffsetHashEntry))
        {
            while((0 == (searchPtr->nextOffsetHashEntry)) && ((searchPtr->nextIndex) < (HASH_TABLE_SIZE-1)))
            {
                searchPtr->nextIndex++;
                (searchPtr->nextOffsetHashEntry) = t->offsetHashEntry[(searchPtr->nextIndex)];
            }
            if(0 == (searchPtr->nextOffsetHashEntry))//reached the end of the table and yet the off is 0
            {
                return FALSE;
            }
            else
            {
                //SHM_Hash_Entry *pNextHashEntry = (SHM_Hash_Entry*)offToPtrHash(t, (searchPtr->nextOffsetHashEntry));
                //(searchPtr->nextOffsetHashEntry) = pNextHashEntry->offsetNext;
                LogDebug("Hash_EnumNext offSet case head %d", searchPtr->nextOffsetHashEntry);
                return TRUE;
            }
        }
        else
        {
            SHM_Hash_Entry *pNextHashEntry = (SHM_Hash_Entry *)offToPtrHash(t, (searchPtr->nextOffsetHashEntry));
            (searchPtr->nextOffsetHashEntry) = pNextHashEntry->offsetNext;
            LogDebug("Hash_EnumNext case chain offSet %d", searchPtr->nextOffsetHashEntry);
            return TRUE;
        }

}

int VidscaleSHMIntf::Hash_getNumEntries(SHM_Hash_Table *t)
{
    if(t)
    {
        return t->tableStats.numEntries;
    }
    return 0;
}


//By using this routine we do not need to use lookup for an existing key
//it will be checked inside with efficient way

bool VidscaleSHMIntf::createRabRecordInSharedMemory(umtsRab *pNewRab)
{
    umtsRab *pOldRabRec = NULL;  
    if(!(createSHMHashEntry((char*)(&(pNewRab->upTeidKey)),SHM_KEY_TUN, pNewRab, pOldRabRec)))
    {
      LogWarn("UPTEID - create entry failed \n");
      return false;
    }
    else
    {
      LogInfo("UPTEID - create entry successful\n");
    }

    if(!( createSHMHashEntry((char*)(&(pNewRab->downTeidKey)),SHM_KEY_TUN, pNewRab,pOldRabRec))) 
    {
      LogWarn("DOWNTEID - create entry failed \n");

      /*Delete uptunnel key*/
      if(!(deleteSHMHashEntry((char*)(&(pNewRab->upTeidKey)), SHM_KEY_TUN, pNewRab->imsi)))
      {
        LogWarn("UPTEID - Old Session deletion failed \n");
      }
      return false;
    }
    else
    {
      LogInfo("DOWNTEID - create entry successful\n");
    }

    //Clean if SHM has conflicted tunnel key rab object represented by
    //pOldRabRec
    if(!pOldRabRec)
    {
      //clean here
    }
    return true;
}

bool VidscaleSHMIntf::removeRabRecordInSharedMemory(umtsRab *pRabObject)
{
    bool retVal = true;

    LogDebug("removeTunnelInSharedMemory\n");
    if(!(deleteSHMHashEntry((char*)(&(pRabObject->upTeidKey)), SHM_KEY_TUN, pRabObject->imsi)))
    {
      LogWarn("UPTEID - Old Session deletion failed \n");
      retVal = false;
    }
    else
    {
      LogInfo("UPTEID - delete entry successful\n");
    }

    if(!(deleteSHMHashEntry((char*)(&(pRabObject->downTeidKey)), SHM_KEY_TUN, pRabObject->imsi)))
    {
      LogWarn("DOWN TEID -Old Session deletion failed \n");
      retVal = false;
    }
    else
    {
      LogInfo("DOWNTEID - delete entry successful\n");
    }

    phashValueFreeMemMgrObj->pushFreeMem(pRabObject , (pRabObject->imsi%MAX_CONTROLLING_ENTITIES)); 
    return retVal;
}

umtsRab* VidscaleSHMIntf::createSHMHashEntry(char* key, ShmKeyType keyType,
                                             umtsRab* pSHMRab, umtsRab*
                                             pOldRabRec)
{
    FirstStrideEntry    *firstStrideEnt = NULL;
    SHM_Hash_Table           *pHashTable = NULL;
    unsigned int keyLength = 0;
    LogDebug("createSHMHashEntry Entry\n");
    FirstStrideBlock    *pFirstStrideBlock = NULL;
    //zeroKeyPadding(key, keyType);

    uint8_t memMgrQueuIdx = (pSHMRab->imsi % MAX_CONTROLLING_ENTITIES);

    uint32_t firstStrideIndex = 0;
    switch(keyType)
    {
      case SHM_KEY_TUN:
      {
        LogDebug("createSHMHashEntry TUN Key\n");
        FirstStrideBlock    *pFirstTunStrideBlock = (FirstStrideBlock *)offToPtr(sm_firstTunnelStrideOffset);
        firstStrideEnt = (FirstStrideEntry *)&(pFirstTunStrideBlock->stride[0]);

        uint32_t fsKey = ipHash(&(((struct RabRecordKey*)key)->dstIP));  //hash key to find entry in the first stride
        fsKey = fsKey & 0x0000FFFF;       //take the 2 LSBytes of the hashed IP address

        firstStrideIndex = jhash(&fsKey, sizeof(fsKey), 10);
        firstStrideIndex = firstStrideIndex & (NUM_TUN_SECOND_STRIDES - 1);
        pFirstStrideBlock = (FirstStrideBlock *)offToPtr(sm_firstTunnelStrideOffset);
        pHashTable = (SHM_Hash_Table *)(offToPtr((firstStrideEnt[firstStrideIndex].offForNextStride)));
        keyLength = sizeof(RabRecordKey);
      }
      break;
      default:
      {
        LogError("Invalid key type\n");
        return NULL;
      }
    }
    int oldSessValueOffset = INALID_OFFSET ;
    umtsRab* pRabRec = NULL;
    if(FALSE == Hash_CreateEntry(pHashTable, key, keyLength, ptrToOff( pSHMRab),
                                 memMgrQueuIdx, &m_shmUpdateLock, &oldSessValueOffset))
    {
      __sync_fetch_and_add(&(pFirstStrideBlock->numCreateFail) , 1);
      pOldRabRec =  (umtsRab*)offToPtrHash(pHashTable, oldSessValueOffset);
      return NULL;
    }

    if(oldSessValueOffset != INALID_OFFSET)
    {
      pOldRabRec =  (umtsRab*)offToPtrHash(pHashTable, oldSessValueOffset);
    }

    __sync_fetch_and_add(&(pFirstStrideBlock->numCreateSuccess) , 1);
    return pSHMRab;
}

bool VidscaleSHMIntf::deleteSHMHashEntry(char* key, ShmKeyType keyType, uint64_t imsi)
{
    FirstStrideEntry    *firstStrideEnt = NULL;
    SHM_Hash_Table          *pHashTable = NULL;          
    unsigned int keyLength = 0;
    LogDebug("deleteSHMHashEntry Entry\n");
    FirstStrideBlock    *pFirstStrideBlock = NULL;
    //zeroKeyPadding(key, keyType);

    uint8_t memMgrQueuIdx = (imsi % MAX_CONTROLLING_ENTITIES);
    switch(keyType)
    {
      case SHM_KEY_TUN:
      {
        FirstStrideBlock    *pFirstTunStrideBlock = (FirstStrideBlock *)offToPtr(sm_firstTunnelStrideOffset);
        firstStrideEnt = (FirstStrideEntry *)&(pFirstTunStrideBlock->stride[0]);

        uint32_t fsKey = 0; //hash key to find entry in the first stride
        fsKey = ipHash(&(((struct RabRecordKey*)key)->dstIP));  //hash key to find entry in the first stride
        fsKey = fsKey & 0x0000FFFF;       //take the 2 LSBytes of the hashed IP address

        uint32_t firstStrideIndex = jhash(&fsKey, sizeof(fsKey), 10);
        firstStrideIndex = firstStrideIndex & (NUM_TUN_SECOND_STRIDES - 1);
        if(firstStrideEnt[firstStrideIndex].offForNextStride == 0)
        {
          LogDebug("Tun Hash table and entry not there\n");
          __sync_fetch_and_add(&(pFirstTunStrideBlock->numDeleteFail) , 1);
          return FALSE;
        }
        pHashTable = (SHM_Hash_Table *)(offToPtr( (firstStrideEnt[firstStrideIndex].offForNextStride)));
        keyLength = sizeof(RabRecordKey);
        pFirstStrideBlock = (FirstStrideBlock *)offToPtr(sm_firstTunnelStrideOffset);
      }
      break;
      default:
      {
        LogError("Invalid key type\n");
        return FALSE;
      }
    }
    umtsRab* pSHMSess = NULL;
    pSHMSess = Hash_DeleteEntry(pHashTable, key, keyLength, memMgrQueuIdx, &m_shmUpdateLock);
    if(NULL == pSHMSess)
    {
      __sync_fetch_and_add(&(pFirstStrideBlock->numDeleteFail) , 1);
      return FALSE;
    }
    else
    {
      __sync_fetch_and_add(&(pFirstStrideBlock->numDeleteSuccess) , 1);
      return TRUE;
    }
}

VidscaleSHMIntf::VidscaleSHMIntf()
{
    m_numOfRabObjs = NUM_RAB_OBJECTS;
    LogNotice("\nmax sessions supported = %u\n",
                m_numOfRabObjs);

    sm_firstTunnelStrideSize   = sizeof(FirstStrideBlock);
    sm_numSecondStride         = NUM_TUN_SECOND_STRIDES ;
    sm_secondStrideSize        = (sizeof(SHM_Hash_Table)*sm_numSecondStride);

    //First stride holds structure that points to Hash Tables(Second Strides)
    sm_firstTunnelStrideOffset = SHM_INFO_SIZE;

    //Hash Tables (Second Strides) points to Hash Objects ()
    sm_secondStrideOffset      = SHM_INFO_SIZE + sizeof(FirstStrideBlock);

    sm_rabObjOffset   = sm_secondStrideOffset + sm_secondStrideSize;
    m_rabObjsSize     = (sizeof(umtsRab)*m_numOfRabObjs);
    LogNotice("size of m_rabObjsSize %u\n", ((m_rabObjsSize/8)/1024));

    //Hash Objects points to Hash Entries (UumtsRab entry here)
    m_hashObjsOffset = sm_rabObjOffset + m_rabObjsSize;
    m_numHashObjs = 2*m_numOfRabObjs; //Single Rab objects can be looked by two keys uplink/downlink 
    m_hashObjsSize = sizeof(SHM_Hash_Entry)*((2*m_numOfRabObjs));
    LogNotice("size of m_hashObjsSize %u\n", ((m_hashObjsSize /8)/1024));



    LogNotice("size of sm_firstTunnelStrideSize %u\n", ((sm_firstTunnelStrideSize /8)/1024));
    LogNotice("size of sm_secondStrideSize %u\n", ((sm_secondStrideSize /8)/1024));

    m_sessionShmSize = SHM_INFO_SIZE +
      sm_firstTunnelStrideSize +
      sm_secondStrideSize +
      m_rabObjsSize +
      m_hashObjsSize;

    LogNotice("size of m_sessionShmSize %u\n", ((m_sessionShmSize /8)/1024));
    pthread_mutexattr_t attributes ;
    pthread_mutexattr_init(&attributes);
    /*
       This type of mutex provides error checking. A thread attempting to relock this mutex without first unlocking it shall return with an error. A thread attempting to unlock a mutex which another thread has locked shall return with an error. A thread attempting to unlock an unlocked mutex shall return with an error. 
     */
    pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_ERRORCHECK);

    if(pthread_mutex_init(&m_shmUpdateLock, &attributes)!= 0)
    {
      perror(__FILE__ "pthread_mutex_init :: shmUpdateLock");
      return;
    }
}

VidscaleSHMIntf::~VidscaleSHMIntf()
{
    if (m_sharedBase)
    {
       munmap(m_sharedBase, m_sessionShmSize);
      shm_unlink(m_sessionShmGlobalKey.c_str());
    }
}

void VidscaleSHMIntf::Init()
{

    LogNotice("\nInitializing memory\n\n");
    int sharedFd = -1;
    int i;
    int keyIndex;
    int tmpFd;

    //First attempt is to see if a shared memory object already exist
    //If object exist unlink that memory so that it can be freed
    // We should close the shm FD after we are through with it.
    // note that an FD does not need to stay open after mmap().
    for(i = 0; i<MAX_SHM_KEYS; i++)
    {
      tmpFd = shm_open(sharedMemKey[i], O_RDWR, S_IRUSR | S_IWUSR);
      if (tmpFd != -1)
      {
        shm_unlink(sharedMemKey[i]);
        close(tmpFd); // close the FD so that we are not holding the old SHM filesystem space
        LogNotice("\n2:Initializing memory\n\n");
      }

      LogNotice("\n3:Initializing memory\n\n");
      if ((sharedFd = shm_open(sharedMemKey[i], 
                               O_RDWR|O_CREAT, S_IRUSR | S_IWUSR)) == -1)
      {
        LogError("Not able to initialize shared memory \n");
        return;   /*should we exit or fall back to normal way ?*/
      }
      else
      { 
        keyIndex = i;
        LogError("Openeing shared memory key(%s) \n", sharedMemKey[keyIndex]);
      }
      LogNotice("\n4:Initializing memory-- size %u\n\n", ((m_sessionShmSize / 8)/ 1024));
    }
    
    if (ftruncate(sharedFd, m_sessionShmSize) == -1)
    {
      LogError("ftruncate failed \n");
      close(sharedFd); // tidy up
      return;
    }

    if ((m_sharedBase = mmap(NULL, m_sessionShmSize, PROT_WRITE | PROT_READ, 
                             MAP_SHARED|MAP_NORESERVE|MAP_LOCKED|MAP_POPULATE , sharedFd, 0)) == (char *)-1) {
      LogError("unable to map the required shared memory \n");
      close(sharedFd); // tidy up
      return;
    }
    else
    {
      LogNotice("MMAP is successful \n\n");
    }

    close(sharedFd); // no more use for FD, so close it

    memset(m_sharedBase, 0, m_sessionShmSize);

    LogNotice("MEMSET is successful \n\n");

    setSessionShmGlobalKey(sharedMemKey[keyIndex]);

    //Set the version of the shared memory
    ShmInfo shmInfo;
    shmInfo.shmVersion = SHM_VERSION;
    memcpy(m_sharedBase, &shmInfo, sizeof(ShmInfo));

   // divideAndInitSharedMemory();
    return;
}

void VidscaleSHMIntf::divideAndInitSharedMemory()
{
    LogError("I am inside divideAndInitSharedMemory \n");

    phashTableFreeMemMgrObj = new FreeMemMgr<SHM_Hash_Table>((SHM_Hash_Table *) offToPtr(sm_secondStrideOffset), sm_numSecondStride);

    phashValueFreeMemMgrObj = new FreeMemMgr<umtsRab>((umtsRab *)offToPtr(sm_rabObjOffset),
                                                         m_numOfRabObjs);
    phashEntryFreeMemMgrObj = new FreeMemMgr<SHM_Hash_Entry>((SHM_Hash_Entry*)offToPtr(m_hashObjsOffset),
                                                             m_numHashObjs);

    /*Allocate all first stride hash tables and initialized them here to avoid
     * lock at later stage (it needs to be protected while allocating hash table
     * memory if not initialized here)*/
    FirstStrideBlock *pFirstRabStrideBlock = (FirstStrideBlock *)offToPtr(sm_firstTunnelStrideOffset);
    pFirstRabStrideBlock->numCreateSuccess = 0;
    pFirstRabStrideBlock->numDeleteSuccess = 0;
    pFirstRabStrideBlock->numCreateFail = 0;
    pFirstRabStrideBlock->numDeleteFail = 0;

    FirstStrideEntry    *firstStrideEnt = (FirstStrideEntry *)&(pFirstRabStrideBlock->stride[0]);
    for(int x =0; x< NUM_TUN_SECOND_STRIDES; x++)
    {
        firstStrideEnt[x].offForNextStride = 0;
        LogDebug("First Stride entry not present. Creating hash table.\n");
        SHM_Hash_Table* pNewHashTable = phashTableFreeMemMgrObj->getFreeMem(0); //Hard coded right now
        if(NULL == pNewHashTable)
        {
          LogError("Memory for Hash Table not available\n");
          return ;
        }
        Hash_InitTable(pNewHashTable, phashEntryFreeMemMgrObj, phashValueFreeMemMgrObj, m_sharedBase);
        firstStrideEnt[x].offForNextStride = ptrToOff(pNewHashTable);

        LogWarn("Index %u Tunnel HashTable Pointer-- %p hashVal(shmp %p--- FreeMemMgr %p)\n",
                  x, pNewHashTable, 
                  pNewHashTable->m_sharedBase,
                  pNewHashTable->phashEntryFreeMemMgrObj);
    }
}
