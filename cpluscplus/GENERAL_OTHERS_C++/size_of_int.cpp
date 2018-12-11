#include <iostream>
#include<stdint.h>
#include "stdio.h"
using namespace std;

struct iphdr {
    unsigned int ihl : 4;
    unsigned int version : 4;
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
};

typedef struct vlanhdr
{
       uint16_t tag;
          uint16_t proto;
}vlanhdr;

int main()
{
   uint32_t z = 10;
   uint64_t s = 30;

   char apnBuf[]= {
      0x02, 0x61, 0x63, 0x08, 0x76, 0x6f, 0x64, 0x61, 0x66, 0x6f, 0x6e, 0x65, 0x02, 0x65, 0x73
    };

  printf("APN buffer %s\n", apnBuf);
    uint32_t m_mtu_ip_payload = ((1500 - sizeof(vlanhdr) - sizeof(iphdr)) >> 2) << 2;
    uint32_t m_ip_payload = ((1467- sizeof(iphdr)) >> 3) << 3;
    uint32_t m_simple_payload = (1500- sizeof(iphdr));
    printf("size of vlanHdr(%u) ipHdr(%u) size of payload(%u) without_vlan(%u) simple(%u) \n",
           sizeof(vlanhdr), sizeof(iphdr), m_mtu_ip_payload, m_ip_payload, m_simple_payload);
return 0;
}
