#include <stdio.h>
#include <iostream>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>

int main()
{
    int n;
    unsigned int m = sizeof(n);
    int fdsocket;
    fdsocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP); // example
    //fdsocket = socket(AF_INET,SOCK_DGRAM,IPPROTO_TCP); // example
    getsockopt(fdsocket,SOL_SOCKET,SO_SNDBUF,(void *)&n, &m);
    printf("Socket receive buffer size is %d\n", n);
    return 0;
}
