#include<stdio.h>
#include<stdint.h>
using namespace std ;

int main()
{
    uint8_t firstByte = 0;
    uint8_t secondByte = 0;

    bool PdpStatus[16];
    for(int x = 0; x<8; x++)
    {
      PdpStatus[x] = true;
    }
    for(int x = 8; x<16; x++)
    {
      PdpStatus[x] = true;
    }
    for(int x=5; x<8; x++)
    {
      if(PdpStatus[x])
      {
        firstByte = firstByte | (uint8_t)1 << x;
      }
      else
      {
        firstByte = firstByte | (uint8_t)0 << x;
      }                                                     
    }                                                       
    for(int x=0; x<8; x++)  
    {
      if(PdpStatus[x+8])
      {
        secondByte = secondByte | (uint8_t)1 << x;
      }
      else
      {
        secondByte = secondByte | (uint8_t)0 << x;
      }
    }
    printf("firstByte(%d)  secondByte(%d) \n",firstByte,secondByte);

    return 0;
}
