#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

bool is_digit_str(const char* str)
{  
   while(*str)
   {  
      if(!isdigit(*str)) { return false; }
      str++;
   }
   return true;
}

bool Parse(const char* ip_colon_port)
{
    int m_port = 0;
    uint32_t m_ip = 0;
  
    char* _ip_colon_port = strdup(ip_colon_port);
    char* strPort = strstr(_ip_colon_port, ":");
    printf("ip_colon_port is %s and strPort is %s \n", _ip_colon_port, strPort);
    if(strPort != 0)
    {  
       strPort++;
       if(!is_digit_str(strPort)) { return false; }
       m_port = atoi(strPort);
       printf("port value is %u\n", m_port);
    }
    
    char *ipStr =  strtok(_ip_colon_port, ":");
    if(ipStr)
    {   
        m_ip = inet_addr(ipStr);
    printf("IP is  is %u\n", m_ip);
    }
    free(_ip_colon_port);
    if(m_ip == 0xFFFFFFFF)
    { 
      return false;
    }
    return true;
}


int main()
{
    char *ptr = 0;
    const char *sptr = ptr;

    while(sptr)
    {
      sptr = "10";
      printf("hi\n");
    }
    //const char *parseStr = "192.168.126.122:movik.net";
    const char *parseStr = "mb.go.biglobe.ne.jp:80";
 //   if(Parse(parseStr))
    {
        printf("parsing success \n");
    }
 //   else
    {
   //     printf("parsing not success\n");
    }
    return 0;
}
