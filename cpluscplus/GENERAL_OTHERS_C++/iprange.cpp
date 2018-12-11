#include<iostream>
#include<stdint.h>
#include<stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define CONFIG_FILE "IpRange.txt"
#define MAX_BUFF_SIZE 256
using namespace std;
typedef struct
{
    std::string pvcName;
    std::string tunDev;
    uint32_t lowerIpStr;
    uint32_t upperIpStr;
}IpRangeInfo_t;
IpRangeInfo_t g_IpRangeInfo[255];
char* ip_to_str(int ip)
{
   struct in_addr inadd;
   inadd.s_addr = ip;
   return inet_ntoa(inadd);
}
uint32_t str_to_ip(const char* str_ip)
{   
    return inet_addr(str_ip);
}   

void HardCodedIpRangeUsed()
{
    FILE *fp;
    int x = 0;
    if ((fp = fopen(CONFIG_FILE, "r")) == (FILE *)0)
    {
      printf("Cannot open file %s for reading ", CONFIG_FILE);
      return ;
    }

    char line[MAX_BUFF_SIZE];
    char str[MAX_BUFF_SIZE];
    while (fgets(line, MAX_BUFF_SIZE, fp) != NULL)
    {
      if (sscanf(line, "%s", str) <= 0)
        continue;

      if (strlen(str) == 0)
        continue;

      char     lowerIpStr[16];
      char     upperIpStr[16];
      char     pvcName[16];
      char     tunDevName[16];
      sscanf(line, "%s %s %s %s", pvcName, lowerIpStr, upperIpStr,tunDevName);
      printf("Read pvcName:=%s, lowerIpStr=%s, upperIpStr=%s, tunDeviceName=%s\n", pvcName, lowerIpStr, upperIpStr,tunDevName);
      g_IpRangeInfo[x].pvcName = pvcName;
      g_IpRangeInfo[x].tunDev = tunDevName;
      printf("Pvc is (%s)\n",g_IpRangeInfo[x].pvcName.c_str());
      g_IpRangeInfo[x].lowerIpStr = inet_addr(lowerIpStr);
      g_IpRangeInfo[x].upperIpStr = inet_addr(upperIpStr);
      uint32_t minIp = str_to_ip(lowerIpStr);
      uint32_t maxIp = str_to_ip(upperIpStr);
        printf("minIp(%u)\n",minIp);
        int loop =0;
      for (uint32_t ueip=ntohl(minIp); ueip<=ntohl(maxIp); ueip++)
      {
        if ((ueip & 0x00000ff) == 0) //network address
        {
          printf("ignoring network address:%x %s\n", ueip, ip_to_str(htonl(ueip)));
          continue;
        }
        if (ueip == 0xaffffff) //broadcast address
        {
         printf("ignoring bcast addr:%x %s\n", ueip, ip_to_str(htonl(ueip)));
          continue;
        }
        //Insert in network byte order
        //pList->push_back(htonl(ueip));
        printf("push back ueIp(%u)\n",htonl(ueip));
        loop++;
        if(loop > 2)
        break;
      }

      x++;
    }
}


int main()
{
    const char *min = "10.0.0.1";
    const char *max = "10.255.255.255";
    uint32_t minIp = str_to_ip(min);
    uint32_t maxIp = str_to_ip(max);
    HardCodedIpRangeUsed();
    //printf("min ip(%u) and max Ip(%u)\n",inet_addr(min),inet_addr(max));
    printf("min ip(%u) and max Ip(%u)\n",minIp,maxIp);

    return 0;
}
