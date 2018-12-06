#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
using namespace std;

static void convertApnStringToEncodableForm(std::string apnStr, char *buff)
{   
    int segLen = 0;
    char *tempPtr = NULL;
    const char *tempStr = apnStr.c_str();
    for(int x = 0; x < apnStr.length() ; x++)
    {
      printf("Byte before converting %x\n", tempStr[x]);
    }
    printf("i am inside 1 %s\n", tempStr);
    for(int i = 0; i < apnStr.length(); i++)
    {
      segLen++;
      if(tempStr[i] == '.')
      {
        segLen--;
        buff[i-segLen] = segLen;
        tempPtr = buff + ((i-segLen) + 1);
        printf("i am inside 2 %s i %d segLen %d %c \n", tempStr, i, segLen, tempStr[i-segLen]);
        memcpy(tempPtr, (const void *)(tempStr + (i-segLen)), segLen);
        printf("tempPtr %s buff %s %d \n", tempPtr, buff, buff[i-segLen]);
        segLen = 0;
      }
    }

    /*Add length for last segment */
    if(apnStr.length() - segLen > 0)
    {
      buff[apnStr.length() - segLen] = segLen;
      tempPtr = buff + ((apnStr.length() - segLen) + 1);
      memcpy(tempPtr, (const void *)(tempStr + (apnStr.length() - segLen)), segLen);
    }

    printf("tempPtr %s buff %s %d \n", tempPtr, buff, buff[apnStr.length() - segLen]);
    for(int x = 0; x <= apnStr.length() ; x++)
    {
      printf("Byte after converting %x\n", buff[x]);
    }
}   

int main()
{
    //string x = "ac.vodafone.uk";
    string x = "www.myspace.uk";
    char buff[100];
    memset(buff, 0, 100);
    printf("i am here\n");
    convertApnStringToEncodableForm(x, buff);
    printf("after converting string is %s\n", buff);

}
