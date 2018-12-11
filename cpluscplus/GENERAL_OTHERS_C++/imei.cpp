#include<iostream>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include <sstream>
using namespace std;

int  encodeIMEI(uint8_t* imeiBuf, uint32_t imeiPrefix, uint32_t ueId)
{   
    int imeiBufLen = 0;
    //123456789000002 //IMEI
    //1234567890000011 //IMEISV

    char imeiStr[20];
    //int imeisvLen = snprintf(imeiStr, sizeof(imeiStr), "%08x%06d10", imeiPrefix, ueId);
    //int imeisvLen = snprintf(imeiStr, sizeof(imeiStr), "123456789000002");
    int imeisvLen = snprintf(imeiStr, sizeof(imeiStr), "1234567890000011");
    printf("imeisvLen %d imeistr(%s) \n",imeisvLen,imeiStr);
    /* Prune the IMEI string */
    for(int index=0; index<imeisvLen; index++)
    {
       if((imeiStr[index] < '0') || (imeiStr[index] > '9'))
       {
          imeiStr[index] = '0';
       }
       imeiStr[index] -= '0';
    }
    if(!(imeisvLen % 2))
    {
      imeiStr[imeisvLen] = 0x0F;
      imeisvLen++;
    printf("hi i am  \n");
    }
    printf("imeisvLen %d \n",imeisvLen);

    imeiBuf[0] = (imeiStr[0] << 4) | 0x08 | 0x3; /*1st digit, odd, imeisv */
    printf("first digit %d (imeiStr[0]<<4)%d \n",imeiBuf[0],(imeiStr[0] << 4));
    imeiBufLen++;

    for(int index=1; index<imeisvLen; index+=2)
    {
       char nextByte = (imeiStr[index+1] << 4) | (imeiStr[index] & 0x0F);
       imeiBuf[imeiBufLen] = nextByte;
       imeiBufLen++;
       printf("imeiBufLen %d\n",imeiBufLen );
    }

    return imeiBufLen;
}
#define GMM_IMEI 2
#define GMM_IMEI_SV 3
bool GmmDecodeImeiOrImeisv(const uint8_t *val, uint32_t len, uint8_t *imei, uint8_t mobileIdentityType)
{
    // first digit is bits 5-8  
    imei[0] = ((val[0] & 0xf0)>>4) + '0';
    imei += 1;
    uint8_t loop_len =0;
    if(mobileIdentityType == GMM_IMEI_SV)
    {
        loop_len = 9;
    }
    else if(mobileIdentityType == GMM_IMEI)
    {
        loop_len = 8;
    }
    else
    {
        return false;
    }
    printf("length %d\n", len);  
    //for(uint32_t i=1; i<len || i <loop_len; i++)
    for(uint32_t i=1;  i < loop_len; i++)
    {
      imei[0] = (val[i] & 0x0f) + '0';
      if (((val[i] & 0xf0)>>4) != 0xf)
      {
        imei[1] = ((val[i] & 0xf0)>>4) + '0';
      }
      else
      {
        imei[1] = '\0';
        break;
      }

      imei += 2;
    }
    return true;
}

uint32_t DecodeIMSI(uint8_t _val[],uint8_t len,uint8_t *imsi)
{
 
    char val[16];
    memcpy(val, _val, len);
    memset(val+1, 0, 3); /* remove IMSI prefix */
    printf("length is %d\n",len);
    for(uint32_t i=2; i<len; i++)
    {
      imsi[0] = (val[i] & 0x0f) + '0';
      printf("value of i %d imsi(%c)\n", i,imsi[0]);  

      if (((val[i] & 0xf0)>>4) != 0xf)
      {
        imsi[1] = ((val[i] & 0xf0)>>4) + '0';
      printf("second..value of i %d imsi(%c)\n", i,imsi[1]);  
      }
      else
      {
        imsi[1] = '\0';
      }

      imsi += 2;
    }
    imsi[0] = 0;
    return 0;
}

int main()
{
    uint64_t m_imei_sv;
    uint8_t imeiBuf[17]= {'\0'};
    uint8_t         sMobileIdentity[11];
    char imeiStr[20];
    uint32_t imeiPrefix=18240512;
    uint32_t ueId=1337384;
    int sMobileIdentityLen = encodeIMEI(sMobileIdentity, imeiPrefix, ueId);
    //imei string=01165400133738410
    printf("imei string [%s] IMEI prefix = %8x ueID is %06d imesv len =%d\n",(char *)sMobileIdentity, imeiPrefix, ueId, sMobileIdentityLen);
    uint8_t mobileIdentityType = GMM_IMEI_SV;
    //uint8_t mobileIdentityType = GMM_IMEI;
    GmmDecodeImeiOrImeisv(sMobileIdentity, sMobileIdentityLen , imeiBuf, mobileIdentityType);
    //DecodeIMSI(sMobileIdentity, sMobileIdentityLen , imeiBuf);

    imeiBuf[16] = '\0';
    m_imei_sv = ::strtoull(((const char *)imeiBuf),0,10); 
    
    std::ostringstream stm;
    std::string tac_string;
    // Output an int
    stm << m_imei_sv;
    // Retrieve the resulting string
    tac_string = stm.str();
    std::cout << tac_string <<endl ;
    

    printf("imeisv = %ju\n",m_imei_sv);
    return 0;
}



