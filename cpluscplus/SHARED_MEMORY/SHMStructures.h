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
 *	@(#)hash.h	8.1 (Berkeley) 6/6/93
 * $FreeBSD: src/usr.bin/make/hash.h,v 1.20 2005/05/13 08:53:00 harti Exp $

 * Copyright (c) 1989 by Berkeley Softworks
 */

#ifndef _SHM_STRUCTURES_H_
#define _SHM_STRUCTURES_H_

#define offToPtrHash(t, a) ((char *)(t->m_sharedBase) + a)
#define ptrToOffHash(t, a) ((char *)a - (char *)(t->m_sharedBase))

#define offToPtr(a) ((char *)m_sharedBase + a)
#define ptrToOff(a) ((char*)a - (char*)m_sharedBase )

#define MAX_SHM_KEYS 1
#define SHM_VERSION 1


#define NUM_TUN_SECOND_STRIDES       256 //should always be a power of 2
//one million entry support
//#define NUM_RAB_OBJECTS              1000*1024 //should always be a power of 2
#define NUM_RAB_OBJECTS              1000 //should always be a power of 2

#define MAX_CONTROLLING_ENTITIES 2

//#define HASH_TABLE_SIZE 64*1024
#define HASH_TABLE_SIZE 1024

#define VALID_VAL 1
#define INVALID_VAL 0

#define VIDSCALE_SHM_SS_IPV4_SIZE     4
#define VIDSCALE_SHM_SS_IPV6_SIZE     16

#define VIDSCALE_SHM_SS_MAXSIZE	 16

#define INALID_OFFSET -1

#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include "assert.h"
#include <time.h>

#define LogNotice printf
#define LogDebug printf
#define LogError printf
#define LogInfo printf
#define LogWarn printf

#define TRUE    1
#define FALSE   0

struct Ipv4v6storage {
	unsigned short	ss_family;		/* address family */
	/* Following field(s) are implementation specific */
	char		__data[VIDSCALE_SHM_SS_MAXSIZE];
				/* space to achieve desired size, */
				/* _SS_MAXSIZE value minus size of ss_family */
}__attribute__((packed));

struct RabRecordKey {
    struct Ipv4v6storage srcIP, dstIP;
    uint32_t teid;
} ;

struct umtsRab
{
	uint64_t imsi:56;
	uint64_t nsapi:8;
	uint16_t lac;
	uint16_t sac;
	uint8_t  ratType;
	uint8_t  validIpKey:1;
	uint8_t  validUpTunKey:1;
	uint8_t  validDownTunKey:1;
	RabRecordKey   upTeidKey;
	RabRecordKey   downTeidKey;
	bool     auditFlag;
}__attribute__((packed));

struct FirstStrideEntry {
    uint16_t refcount;
    uint32_t offForNextStride;
};

struct FirstStrideBlock {
    uint64_t numCreateSuccess;
    uint64_t numDeleteSuccess;
    uint64_t numCreateFail;
    uint64_t numDeleteFail;
    struct FirstStrideEntry stride[NUM_TUN_SECOND_STRIDES];  
};

struct ShmInfo {
    uint32_t shmVersion;
    uint32_t spares;
};

enum ShmKeyType {
    SHM_KEY_IP,
    SHM_KEY_TUN,
    SHM_KEY_INVALID
} ;

struct SHM_Hash_Entry {
    int             offsetNext;     /* Offset of the next entry within same bucket. 0 means "No Entry"*/
    int             offsetValue;    /* Offset of the Value/Session it is pointing to. 0 means "No Entry"*/
    union hashKey_t
    {
        struct RabRecordKey      tunKey;
    }hashKey;
    int             keySize;
} ;


struct Hash_Table_Stats {
    int     numEntries;            /* Number of entries in the table. */
    int     numDeleteNoEntryFound; 
} ;

struct SHM_Hash_Table {
    int                            offsetHashEntry[HASH_TABLE_SIZE]; /* offset == 0 mean "No Entry" */
    void                           *phashEntryFreeMemMgrObj;
    void                           *phashValueFreeMemMgrObj;
    Hash_Table_Stats               tableStats;
    void                           *m_sharedBase;
} ;


template<typename T>
class FreeMemMgr
{
public:

    FreeMemMgr(T *t, int num)
    {
      /*initialize vector*/
      for(int x = 0; x < MAX_CONTROLLING_ENTITIES ; x++)
      {
          std::queue<T *> queueX;
          freeQVector.push_back(queueX);
      }

      int quotient = 0;
      int remainder = 0;
      poolStartPtr = t;
      maxPoolSize = num;
      poolEndPtr = t+num-1;

      LogNotice("Total session objects %u\n", num);
      quotient =  num/MAX_CONTROLLING_ENTITIES;
      remainder =  num%MAX_CONTROLLING_ENTITIES;
      for(int x = 0; x< MAX_CONTROLLING_ENTITIES; x++)
      {
        for (int i=quotient*x; i< (quotient*x + quotient); i++) {
          if(i==quotient*x)
          {
            LogNotice("value of i(%u) start pointer(%p) for index %u --- orgStartPointer %p --- endPointer%p\n",
                      i, t, x, poolStartPtr, poolEndPtr);
          }
          freeQVector.at(x).push(t);
          /* initial time to zero since all the node are created*/
          t++;
        }
        LogNotice("End pointer(%p) for index %u -- orgStartPointer %p --- endPointer%p\n",
                  (t-1), x, poolStartPtr, poolEndPtr);
      }

      for(int i = 0; i < remainder; i++)
      {
        freeQVector.at(0).push(t);
        t++;
      }
      for(int x = 0; x< MAX_CONTROLLING_ENTITIES; x++)
      {
        LogNotice("Size for queue at index (%u)  is %Zu \n", x, freeQVector.at(x).size());
      }
      m_cachedTime = time(NULL);

    }

    T * getFreeMem(uint8_t index)
    {
      if(index >= MAX_CONTROLLING_ENTITIES)
      {
        assert(0);
      }

      if (freeQVector.at(index).empty()){
        return NULL;
      }

      T *str;
      str = freeQVector.at(index).front();
      if (!str) {
        return NULL;
      }
      freeQVector.at(index).pop();
      memset(str,0,sizeof(T));
      return str;
    }

    void pushFreeMem(T *t, uint8_t index)
    {
      if(index >= MAX_CONTROLLING_ENTITIES)
      {
        assert(0);
      }
      freeQVector.at(index).push(t);
      if((t<poolStartPtr)||(t>poolEndPtr))
      {
        assert(0);
      }
    }

    uint32_t freeQueueSize()
    {
            uint32_t size = 0;
            for(int x = 0; x <MAX_CONTROLLING_ENTITIES; x++)
            {
                    size+=freeQVector.at(x).size();
            }
            return size;
    }

    uint32_t maxQueueSize() { return maxPoolSize; }

private:
    uint32_t    maxPoolSize;
    time_t m_cachedTime;
    std::queue<T *> freeQ;
    std::vector< std::queue<T*> > freeQVector;
    T*          poolStartPtr;
    T*          poolEndPtr;
};


struct Hash_Search {
        SHM_Hash_Table      *tablePtr;
	int             indexTable;	        /* Index of the Hash Table being searched in the frist stride. */
	int		nextIndex;	        /* Next bucket to check */
	int      	nextOffsetHashEntry;	/* Next entry in current bucket */
};

#if 0
inline void zeroKeyPadding(char* key, ShmKeyType keyType)
{
    switch(keyType)
    {
      case SHM_KEY_IP:
      {
            if(AF_INET == ((struct ipKey*)key)->IP.ss_family)
            {
                uint32_t* pExt = (uint32_t*)(((struct ipKey*)key)->IP.__data);
                pExt[2] = 0;
                pExt[3] = 0;
            }
      }
      case SHM_KEY_TUN:
      {
            if(AF_INET == ((struct RabRecordKey*)key)->srcIP.ss_family)
            {
                uint32_t* pExt = (uint32_t*)(((struct RabRecordKey*)key)->srcIP.__data);
                pExt[2] = 0;
                pExt[3] = 0;
            }
            if(AF_INET == ((struct RabRecordKey*)key)->dstIP.ss_family)
            {
                uint32_t* pExt = (uint32_t*)(((struct RabRecordKey*)key)->dstIP.__data);
                pExt[2] = 0;
                pExt[3] = 0;
            }
      }
      default:
      return;
    }
}

#endif


#endif
