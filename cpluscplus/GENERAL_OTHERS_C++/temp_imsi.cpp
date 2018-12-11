#include<iostream>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string>
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
    printf("Word value is:%d\n",val);
    return val;
}

uint8_t encodeIMSI(uint32_t tfwId,uint8_t sMobileIdentity[])
{
    unsigned int x=0;
     int p =0;
    uint8_t rem1 = 0,rem2 = 0,byte =0;;
    uint32_t temp = tfwId;
    uint8_t digits[20];
#if 1
    while(temp!=0)
    {
      rem1 = temp%10;
      temp = temp/10;
      digits[p++] = rem1;
      printf("value of digit is(%d)\n",digits[p-1]);
    }
    printf("value of p is(%d)\n",p);
    x=1;
    p--;
    while(p>=0)
    {
      rem1 = digits[p--];
      rem2 = digits[p--];
      printf("rem1 (%d) rem2 (%d)\n",rem1,rem2);
      byte  = rem2 << 4 | rem1;
      sMobileIdentity[x] = byte;
      printf("encoded imsi byte is(%d)\n",sMobileIdentity[x]);
      x++;
    }
#else
    x=1;
    while(temp!=0)
    {
      rem1 = temp%10;
      temp = temp/10;
      rem2 = temp%10;
      temp = temp/10;
      printf("rem1 (%d) rem2 (%d)\n",rem1,rem2);
      byte  = rem2 << 4 | rem1;
      sMobileIdentity[x] = byte;
      printf("encoded imsi byte is(%d)\n",sMobileIdentity[x]);
      x++;
    }
#endif
    if(x<8)
    {
      sMobileIdentity[0] = x-1;
    }
    else
    {
      printf("TFW ID crossed imsi octets limits(%d)\n",tfwId);
    }
    /*encode remaining octets of imsi */
   printf("x is %d and length is(%d)\n",x,sMobileIdentity[0]);
   return 0;
}

uint8_t decodeIMSI(uint8_t val[],uint8_t len,uint8_t *imsi)
{
    for (uint32_t i=0; i<len; i++)
    {
      imsi[2*i] = (val[i] & 0x0f) + '0';

      if (((val[i] & 0xf0)>>4) != 0xf)
      {
        imsi[2*i+1] = ((val[i] & 0xf0)>>4) + '0';
      }
      else
      {
        imsi[2*i+1] = '\0';
      }
    }
   return 0;
}

int main()
{
    uint8_t sMobileIdentity[11];
    uint8_t imsiBuf[17] = {'\0'};
    uint32_t tfwId = 123456;
    uint32_t dectfwId = 0;
    printf("********before encoding tfwId(%d*****\n",tfwId);
    encodeIMSI(tfwId,sMobileIdentity);

    uint8_t len = sMobileIdentity[0];
    decodeIMSI((sMobileIdentity+1),len,imsiBuf);
    string imsiStr((const char *)imsiBuf);
    printf( "IMSI in commonId:%s\n", imsiStr.c_str());
    
    printf("********after decoding tfwId(%d*****\n",atoi(imsiStr.c_str()) );

    return 0;
}
