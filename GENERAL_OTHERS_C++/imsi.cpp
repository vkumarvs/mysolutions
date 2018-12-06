#include<iostream>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string>
using namespace std;
uint8_t encodeIMSI(uint32_t tfwId,uint8_t sMobileIdentity[])
{

    unsigned int x=0;
    int p =0;
    uint8_t rem1 = 0,rem2 = 0,byte =0;;
    uint32_t temp = tfwId;
    uint8_t digits[20];
    if(temp == 0)
    {
      digits[p++] = 0;
      //printf("temp::value of digit is(%d)\n",digits[p-1]);
      
    }
    else
    {
      while(temp!=0)
      {
        rem1 = temp%10;
        temp = temp/10;
        digits[p++] = rem1;
      //  printf("value of digit is(%d)\n",digits[p-1]);
      }
    }
    //printf("value of p is(%d)\n",p);
    x=1;
    p--;
    while(p>=0)
    {
      if(tfwId < 10 || p == 0)
      {
        rem1 = digits[p--];
        rem2 = 0x0f;
      }
      else
      {
        rem1 = digits[p--];
        rem2 = digits[p--];
      }
      printf("rem1 (%d) rem2 (%d)\n",rem1,rem2);
      byte  = rem2 << 4 | rem1;
      sMobileIdentity[x] = byte;
      //printf("encoded imsi byte is(%d)\n",sMobileIdentity[x]);
      x++;
    }
    if(x<8)
    {
      sMobileIdentity[0] = x-1;
    }
    else
    {
      printf("TFW ID crossed imsi octets limits(%d)\n",tfwId);
    }
    /*encode remaining octets of imsi */
    //printf("imsi length is(%d)\n",sMobileIdentity[0]);
    return 0;
}

uint8_t decodeIMSI(uint8_t val[],uint8_t len,uint8_t *imsi)
{
#if 0
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
#else
   char val[16];
   memcpy(val, _val, len);
   memset(val+1, 0, 3); /* remove IMSI prefix */

   for(uint32_t i=1; i<len; i++)
   {
     imsi[0] = (val[i] & 0x0f) + '0';

     if (((val[i] & 0xf0)>>4) != 0xf)
     {
       imsi[1] = ((val[i] & 0xf0)>>4) + '0';
     }
     else
     {
       imsi[1] = '\0';
     }

     imsi += 2;
   }
   imsi[0] = 0;
   return 0;

#endif
}

int main()
{
    uint8_t sMobileIdentity[11];
    uint8_t imsiBuf[11];
    memset(imsiBuf,0,11);
    uint32_t tfwId;
    for(int t=1; t<=20; t++)
    {
      tfwId = t;
      printf("********before encoding tfwId(%d*****\n",tfwId);
      encodeIMSI(tfwId,sMobileIdentity);

      uint8_t len = sMobileIdentity[0];
      printf("decoding::length is(%d)\n",len);
      decodeIMSI((sMobileIdentity+1),len,imsiBuf);
      string imsiStr((const char *)imsiBuf);
      //printf( "\nIMSI in commonId:%s\n\n\n", imsiStr.c_str());
      printf("********after decoding tfwId(%d*****\n",atoi(imsiStr.c_str()) );
    }
    return 0;
}
