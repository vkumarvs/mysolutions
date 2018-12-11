#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

struct in6_addr_1 {
    union {
       uint8_t u6_addr8[16];
        uint16_t u6_addr16[8];
        uint32_t u6_addr32[4];
    } in6_u;
};

int main()
{
    char upLinkstr[INET6_ADDRSTRLEN];
    char downlinkstr[INET6_ADDRSTRLEN];
    char ipstr[INET6_ADDRSTRLEN];
    memset(upLinkstr, 0, INET6_ADDRSTRLEN);
    memset(downlinkstr, 0, INET6_ADDRSTRLEN);
    memset(ipstr, 0, INET6_ADDRSTRLEN);

    //127.165.222.27 -- ipaddress
   //UEID = 607000000000010
   //Rat Type 3
   //session Index 5
   //UPTEID = 2281701385;
   //DOWNTEID = 18
   //DLIP = 2001::8820:0:0:101
   //UPIP = 2002:2::100:0:0:1
   
    in6_addr_1 m_dlTunIpAddr, m_ulTunIpAddr; 

    m_dlTunIpAddr.in6_u.u6_addr32 = {288, 0, 8328, 16842752};

    m_ulTunIpAddr.in6_u.u6_addr32 = {33554976, 0, 1, 16777216};

    inet_ntop(AF_INET6, &(m_dlTunIpAddr), downlinkstr, INET6_ADDRSTRLEN);

    inet_ntop(AF_INET6, &(m_ulTunIpAddr), upLinkstr, INET6_ADDRSTRLEN);

    //   inet_ntop(AF_INET6, &(m_ulTunIpAddr), ipstr, INET6_ADDRSTRLEN);

    printf("DownLink ip addr %s\n", downlinkstr); 
    printf("UpLink Ip Addr %s\n", upLinkstr); 
    return 0;
}

