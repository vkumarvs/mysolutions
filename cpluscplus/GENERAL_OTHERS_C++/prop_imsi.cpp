#include<iostream>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string>
using namespace std;
uint8_t encodeIMSI(uint32_t tfwId,uint8_t sMobileIdentity[])
{
    unsigned int length = 1;
    uint8_t rem1 = 0,rem2 = 0,byte =0,no_of_digit = 0;
    uint32_t temp = tfwId,temp1=tfwId;
    uint8_t digits[20];
    int p =0;
    /*Count no of digits */
#if 0
    if(temp1 == 0)
    {
      p++;
    }
    else
    {
      while(temp1!=0)
      {
        rem1 = temp1%10;
        temp1 = temp1/10;
        p++ ;
      }
    }
      int pading = 4-p;
    if(p<4)
    {
      for(int l = 0; l < pading ; l++)
      {
        digits[l] = 0;
        p++;
      }
    }
#endif
#if 0
      for(int l = 0; l < 4 ; l++)
      {
        digits[l] = 0;
        p++;
      }
#endif
    //int pos = pading;
    int pos = p;
    if(temp == 0)
    {
      digits[pos++] = 0;
    }
    else
    {
      while(temp!=0)
      {
        rem1 = temp%10;
        temp = temp/10;
        digits[pos++] = rem1;
      }
    }
    no_of_digit = pos;
    p = pos;
    printf("p is(%d) no of digit(%d)\n",p,no_of_digit);
    /*odd even digits */
    if(no_of_digit%2 == 0) /*means imsi contain even no of digits */
    {
      rem1 = 0x01;
      rem2 = digits[p-1];
      byte  = rem2 << 4 | rem1;
      sMobileIdentity[length] = byte;
      length++;
      p = p-1;
    }
    else
    {
      rem1 = 0x09;
      rem2 = digits[p-1];
      byte  = rem2 << 4 | rem1;
      sMobileIdentity[length] = byte;
      length++;
      p = p-1;
    }
    p--;
    while(p>=0)
    {
      if(p==0 && !(no_of_digit%2)) /*Encode last digit */
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
      sMobileIdentity[length] = byte;
      printf("encoded imsi byte is(%d)\n",sMobileIdentity[length]);
      length++;
    }
      sMobileIdentity[length++] = 0x00;
      sMobileIdentity[length++] = 0x00;
    if(length<=8)
    {
      sMobileIdentity[0] = length-1;
    }
    else
    {
      printf("TFW ID crossed imsi octets limits(%d) length(%d)\n",tfwId,length);
    }
    /*encode remaining octets of imsi */
    printf("imsi length is(%d)\n",sMobileIdentity[0]);
    return 0;
}

uint8_t decodeIMSI(uint8_t val[],uint8_t len,uint8_t *imsi)
{
   // printf("length is(%d) first byte is(%d)\n",len,val[0]);
    for (uint32_t i=0; i<len-2; i++)
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
    uint8_t imsiBuf1[17] = {'\0'};
    uint32_t tfwId;
    for(int t=279489191; t<=279489193; t++)
    {
      tfwId = t;
      //printf("********before encoding tfwId(%d*****\n",tfwId);
      encodeIMSI(tfwId,sMobileIdentity);

      uint8_t len = sMobileIdentity[0];
     // printf("decoding::length is(%d)\n",len);
      decodeIMSI((sMobileIdentity+1),len,imsiBuf);
      string imsiStr((const char *)imsiBuf);
      printf( "\nIMSI in commonId:%s %s\n\n\n",imsiBuf, imsiStr.c_str());
      //printf("********after decoding tfwId(%d*****\n",atoi(imsiStr.c_str()) );
    }
    return 0;
}
