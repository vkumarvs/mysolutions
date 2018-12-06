#include <stdio.h>
#include <iostream>
#include "math.h"
using namespace std;
#if 0
enum commResourceId                                                                                                   
{                                                                                                                     
    RESOURCE_MGMT_ETHERNET_PORT=1,                                                                               
    RESOURCE_MGMT_ETHERNET_EPL=2,                                                                                
    RESOURCE_MGMT_IP_INTERFACE=4,                                                                                
    RESOURCE_MGMT_IP_ADDR=8,                                                                                     
    RESOURCE_MGMT_CARD=16,                                                                                       
    RESOURCE_MGMT_PB_SWB_CONN=32,                                                                                
};                                    

enum resourceId
{     
    RESOURCE_MGMT_ETHERNET_PORT=1,                                   
    RESOURCE_MGMT_ETHERNET_EPL=2,                               
    RESOURCE_MGMT_IP_INTERFACE=4,                   
    RESOURCE_MGMT_IP_ADDR=8,                                   
    RESOURCE_MGMT_CARD=16,                                
};   
#endif
#define MAX_LOAD_LIMIT_PER_THREAD 1200
int main()
{
    int loadRate  = 1400;
    int numberOfthreads = ceil((float)loadRate/MAX_LOAD_LIMIT_PER_THREAD);
    printf("numberOfthreads %d\n", numberOfthreads);
#if 0
    int x = 10;
    bool p = false;
    int i = 1;
    while(p && i <= x)
    {
      printf("I am vipin \n");
      i++;
    }
#endif
    return 0;
}
