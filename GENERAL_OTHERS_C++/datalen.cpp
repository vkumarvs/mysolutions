#include "stdio.h"
#include "linux/socket.h"
using namespace std;
typedef struct 
{
    unsigned int x;
    unsigned char y;
    unsigned char test[10];
}Length;

struct tunKey {

      struct sockaddr_storage srcIP, dstIP;
      uint32_t teid;
} ;


bool createShmTunKey(uint32_t teid, struct sockaddr_storage* srcAddr, struct sockaddr_storage* dstAddr, struct tunKey *key)
{   
      
    memset(key, 0, sizeof(struct tunKey));
    //fill src Tun Ip
    if (srcAddr->ss_family == AF_INET)
    {
        ((struct sockaddr_in*)(&(key->srcIP)))->sin_addr = ((struct sockaddr_in*)srcAddr)->sin_addr;
        ((struct sockaddr_in*)(&(key->srcIP)))->sin_port = ((struct sockaddr_in*)srcAddr)->sin_port;
        key->srcIP.ss_family = AF_INET;
    }   
    else if(srcAddr->ss_family == AF_INET6)
    {
        ((struct sockaddr_in6*)(&(key->srcIP)))->sin6_addr =  ((struct sockaddr_in6*)srcAddr)->sin6_addr;
        ((struct sockaddr_in6*)(&(key->srcIP)))->sin6_port = ((struct sockaddr_in6*)srcAddr)->sin6_port;
        key->srcIP.ss_family = AF_INET6;
    }
    //fill dst tun ip
    if (srcAddr->ss_family == AF_INET)
    {       
        ((struct sockaddr_in*)(&(key->dstIP)))->sin_addr = ((struct sockaddr_in*)dstAddr)->sin_addr;
        ((struct sockaddr_in*)(&(key->dstIP)))->sin_port = ((struct sockaddr_in*)dstAddr)->sin_port;
        key->dstIP.ss_family = AF_INET;
    }
    else if(srcAddr->ss_family == AF_INET6)
    {
        ((struct sockaddr_in6*)(&(key->dstIP)))->sin6_addr =  ((struct sockaddr_in6*)dstAddr)->sin6_addr;
        ((struct sockaddr_in6*)(&(key->dstIP)))->sin6_port = ((struct sockaddr_in6*)dstAddr)->sin6_port;
        key->dstIP.ss_family = AF_INET6;
    }

    //fill tunnel
    key->teid = teid;
    return true;
}

int main()
{
    tunKey key1, key2;

    Length t;
 //   printf("size of structure is (%d) unsigned int(%d) unsigned char(%d)\n",sizeof(Length) ,sizeof(unsigned int),sizeof(unsigned char));
    return 0;
}
