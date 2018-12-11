#include <iostream>
#include <stdio.h>
#include <stdint.h>
using namespace std;
unsigned int  makeU32Word(uint8_t arr[])
{
    unsigned int val = 0,tmp = 0;
    tmp = arr[0];
    val = val | tmp;
    tmp = 0;
    tmp = arr[1];
    tmp = tmp << 8;
    val = val | tmp;
    tmp = 0;
    tmp = arr[2];
    tmp = tmp << 16;
    val = val | tmp;
    tmp = 0;
    tmp = arr[3];
    tmp = tmp << 24;
    val = val | tmp;
    return val;
}
uint8_t extractByte(uint32_t tfwId,uint8_t pos)
{
    uint32_t val = 0;
    uint8_t byte;
    switch(pos)
    {
      case 1:
      val = tfwId & 0x000000ff;
      byte = (uint8_t)val;
      break;
      case 2:
      val = tfwId & 0x0000ff00;
      val = val >> 8;
      byte = (uint8_t)val;
      break;
      case 3:
      val = tfwId & 0x00ff0000;
      val = val >> 16;
      byte = (uint8_t)val;
      break;
      case 4:
      val = tfwId & 0xff000000;
      val = val >> 24;
      byte = (uint8_t)val;
      break;
      default:
       printf("error\n");
    }
    return byte;
}

int main()
{

    uint32_t tfwId = 532;
    uint8_t identity[10];
    unsigned int tfwId12 ;
    uint8_t pTfwId[4] ;

    unsigned int x=0;
    for(x=1; x<sizeof(unsigned int)+1; x++)
    {
      identity[x] = extractByte(tfwId,x);
      printf("extracted byte is (%d)\n",identity[x]);
    }

    for(x=1; x<sizeof(unsigned int)+1; x++)
    {
      pTfwId[x-1] = identity[x];
      printf("decoded byte is (%d)\n",pTfwId[x-1]);
    }
    tfwId12 = makeU32Word(pTfwId);
    printf("decoded tfwId is (%d)\n",tfwId12);
    return 0;
}
