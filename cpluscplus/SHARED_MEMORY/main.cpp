#include <iostream>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include "VidscaleSHMIntf.h"
using namespace std;

bool generateRabKey(uint32_t teid, void *srcAddr, void *dstAddr, unsigned short
                     ss_family, RabRecordKey *key)
{
	if(!key)
	{
		return false;
	}

	memset(key, 0, sizeof(RabRecordKey));

	//fill src Tun Ip
	if (ss_family == AF_INET)
	{
		memcpy(key->srcIP.__data, &((struct in_addr*)srcAddr)->s_addr, VIDSCALE_SHM_SS_IPV4_SIZE);
		key->srcIP.ss_family = AF_INET;
	}
	else if(ss_family  == AF_INET6)
	{
		memcpy(key->srcIP.__data, ((struct in6_addr*)srcAddr)->s6_addr, VIDSCALE_SHM_SS_IPV6_SIZE);
		key->srcIP.ss_family = AF_INET6;
	}
	//fill dst tun ip
	if (ss_family == AF_INET)
	{
		memcpy(key->dstIP.__data, &((struct in_addr*)dstAddr)->s_addr, VIDSCALE_SHM_SS_IPV4_SIZE);
		key->dstIP.ss_family = AF_INET;
	}
	else if(ss_family  == AF_INET6)
	{
		memcpy(key->dstIP.__data, ((struct in6_addr*)dstAddr)->s6_addr, VIDSCALE_SHM_SS_IPV6_SIZE);
		key->dstIP.ss_family = AF_INET6;
	}

	//fill tunnel
	key->teid = teid;
	return true;
}

bool generateRabKey(uint32_t teid, struct sockaddr_storage *srcAddr, struct sockaddr_storage *dstAddr, RabRecordKey *key)
{
	if(!key)
	{
		return false;
	}

	memset(key, 0, sizeof(RabRecordKey));
	//fill src Tun Ip
	if (srcAddr->ss_family == AF_INET)
	{
		memcpy(key->srcIP.__data, &((struct sockaddr_in*)srcAddr)->sin_addr.s_addr, VIDSCALE_SHM_SS_IPV4_SIZE);
		key->srcIP.ss_family = AF_INET;
	}
	else if(srcAddr->ss_family == AF_INET6)
	{
		memcpy(key->srcIP.__data, ((struct sockaddr_in6*)srcAddr)->sin6_addr.s6_addr, VIDSCALE_SHM_SS_IPV6_SIZE);
		key->srcIP.ss_family = AF_INET6;
	}
	//fill dst tun ip
	if (dstAddr->ss_family == AF_INET)
	{
		memcpy(key->dstIP.__data, &((struct sockaddr_in*)dstAddr)->sin_addr.s_addr, VIDSCALE_SHM_SS_IPV4_SIZE);
		key->dstIP.ss_family = AF_INET;
	}
	else if(dstAddr->ss_family == AF_INET6)
	{
		memcpy(key->dstIP.__data, ((struct sockaddr_in6*)dstAddr)->sin6_addr.s6_addr, VIDSCALE_SHM_SS_IPV6_SIZE);
		key->dstIP.ss_family = AF_INET6;
	}

	//fill tunnel
	key->teid = teid;
	return true;
}

sockaddr_storage* convert_stdstring_2_sockaddr_storage(sockaddr_storage* addr, const std::string* ipStr, uint16_t port)
{
   bzero(addr, sizeof(sockaddr_storage));
   if (ipStr->size() == strspn(ipStr->c_str(), "0123456789."))
   {
      //IPv4 address
      addr->ss_family = AF_INET;
      ((sockaddr_in*)addr)->sin_port = htons(port);
      inet_pton(AF_INET, ipStr->c_str(), &((sockaddr_in*)addr)->sin_addr);
   }
   else
   {
      //IPv6 address
      addr->ss_family = AF_INET6;
      ((sockaddr_in6*)addr)->sin6_port = htons(port);
      inet_pton(AF_INET6, ipStr->c_str(), &((sockaddr_in6*)addr)->sin6_addr);
   }
   return (addr);
}

bool convertStdStrToShmTunKey(string * srcIp, string* dstIp, uint32_t teId , RabRecordKey *key)
{
    if(!key)
    {
        return false;
    }

    memset(key, 0, sizeof(RabRecordKey));
    sockaddr_storage srcAddr;
    sockaddr_storage dstAddr;
    convert_stdstring_2_sockaddr_storage(&srcAddr, srcIp, 0);
    convert_stdstring_2_sockaddr_storage(&dstAddr, dstIp, 0);

    generateRabKey(teId, &srcAddr, &dstAddr, key);

    return true;
}

char *shm_ipv4v6_str(const Ipv4v6storage *sa, char *s, size_t maxlen=INET6_ADDRSTRLEN)
{
   bzero(s, maxlen);
   switch(sa->ss_family) {
        case AF_INET:
            inet_ntop(AF_INET, (sa->__data), s, maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, (sa->__data), s, maxlen);
            break;

        default:
            return s;
    }
    return s;
}

char *get_ipv4v6_str(const Ipv4v6storage *sa, char *s, size_t maxlen=INET6_ADDRSTRLEN, bool debug=false)
{
   bzero(s, maxlen);
   switch(sa->ss_family) {
        case AF_INET:
            inet_ntop(AF_INET, (sa->__data), s, maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, (sa->__data), s, maxlen);
            break;

        default:
            return s;
    }
    return s;
}

int main()
{
    VidscaleSHMIntf::getInstance().Init();
    RabRecordKey upTeidkey, downTeidKey;
    uint32_t upteid = 100;
    uint32_t downTeid = 200;
    string srcIp("10.1.1.2");
    string dstIp("11.1.1.2");
    uint64_t imsi = 1234567891;
    convertStdStrToShmTunKey(&srcIp, &dstIp, upteid, &upTeidkey);
    convertStdStrToShmTunKey(&dstIp, &srcIp, downTeid, &downTeidKey);

    char keySrcIpAddrStr[INET6_ADDRSTRLEN];
    char keyDstIpAddrStr[INET6_ADDRSTRLEN];

    printf("UPTEID KEY TEID(%u) SRCIP(%s) DSTIP(%s)\n", upTeidkey.teid,
             shm_ipv4v6_str(&(upTeidkey.srcIP), keySrcIpAddrStr, INET6_ADDRSTRLEN),
             shm_ipv4v6_str(&(upTeidkey.dstIP), keyDstIpAddrStr, INET6_ADDRSTRLEN));

    printf("DOWNTEID KEY TEID(%u) SRCIP(%s) DSTIP(%s)\n", downTeidKey.teid,
             get_ipv4v6_str(&(downTeidKey.srcIP), keySrcIpAddrStr, INET6_ADDRSTRLEN),
             get_ipv4v6_str(&(downTeidKey.dstIP), keyDstIpAddrStr, INET6_ADDRSTRLEN));

#if 0
    umtsRab *testRab = VidscaleSHMIntf::getInstance().getHashValueMemMgrObj()->getFreeMem((imsi %
                                                           MAX_CONTROLLING_ENTITIES));
    if(NULL == testRab)
    {
      printf("Memory for session object not available\n");
      return 0;
    }

    testRab->upTeidKey    = upTeidkey;
    testRab->downTeidKey  = downTeidKey;

    printf("UPTEID KEY TEID(%u) SRCIP(%s) DSTIP(%s)\n", testRab->upTeidKey.teid,
             shm_ipv4v6_str(&(testRab->upTeidKey.srcIP), keySrcIpAddrStr, INET6_ADDRSTRLEN),
             shm_ipv4v6_str(&(testRab->upTeidKey.dstIP), keyDstIpAddrStr, INET6_ADDRSTRLEN));

    printf("DOWNTEID KEY TEID(%u) SRCIP(%s) DSTIP(%s)\n", testRab->downTeidKey.teid,
             get_ipv4v6_str(&(testRab->downTeidKey.srcIP), keySrcIpAddrStr, INET6_ADDRSTRLEN),
             get_ipv4v6_str(&(testRab->downTeidKey.dstIP), keyDstIpAddrStr, INET6_ADDRSTRLEN));

    //VidscaleSHMIntf::getInstance().createRabRecordInSharedMemory(testRab);

    //VidscaleSHMIntf::getInstance().removeRabRecordInSharedMemory(testRab);
   #endif
    return 0;
}

