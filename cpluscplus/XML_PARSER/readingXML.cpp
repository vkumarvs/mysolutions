#include <iostream>
#include "tinyxml.h"
#include "tinystr.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
void PrintHexDump(const unsigned char* buf, const uint16_t length)
{
   if (!buf)
   {
     printf("Buffer pointer is NULL\n");
     return;
   }
   printf("Buffer Size: %d bytes\n\n", length);
   uint16_t index = 0;
   while(index<length)
   {

      if ( (length - index) >= 8)
      {
        printf(   "|             0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x,  \n",
                        (int)(buf[index]) & 0xFF,
                        (int)(buf[index+1]) & 0xFF,
                        (int)(buf[index+2]) & 0xFF,
                        (int)(buf[index+3]) & 0xFF,
                        (int)(buf[index+4]) & 0xFF,
                        (int)(buf[index+5]) & 0xFF,
                        (int)(buf[index+6]) & 0xFF,
                        (int)(buf[index+7]) & 0xFF
                        );
        index = index + 8;
      }
      else if ( (length - index) >= 2)
      {
        printf(      "|             %02x, %02x\n",
                        (int)(buf[index]) & 0xFF,
                        (int)(buf[index+1]) & 0xFF);
        index = index + 2;
        continue;
      }
      else
      {
        printf("  %02x,  ", (int)(buf[index]) & 0xFF);
        index++;
      }

   }
   printf("\n\n");
}
char* cli_trim_line(char* p_line)
{
    int len = 0;
    
    while(isspace(*p_line)){ p_line++;}
    
    if(!*p_line)
    {
      return p_line;
    } 
    
    len = strlen(p_line) - 1;

    while(len)
    {
      if(isspace(p_line[len]))
      {
        p_line[len]  = 0;
      } else
      {
        break;
      } 
      len--;
    } 
      
    return p_line;
}
void parseXML()
{
    string msgBytes, msgType, procesInfo;
    // Load the xml file, I put your XML in a file named test.xml
    TiXmlDocument XMLdoc("demo.xml");
    bool loadOkay = XMLdoc.LoadFile();
    if (loadOkay)
    {
      cout << "demo.xml loaded" << endl;
      TiXmlElement *pRoot, *pParm1, *pParm2,*pParm3;
      TiXmlElement *pMsgInfo,*pMsgParam;
      pRoot = XMLdoc.FirstChildElement( "Configuration" );
      if ( pRoot )
      {
        pParm1 = pRoot->FirstChildElement("Parameter");
        while ( pParm1 )
        {
          int ID = atoi(pParm1->Attribute("ID"));
          switch(ID)
          {
            case 1:
            default:
            cout << "Parameter: name="  << ID << endl;
          }
          pParm1 = pParm1->NextSiblingElement( "Parameter" );
        }
        cout << endl;

        pMsgInfo = pRoot->FirstChildElement("UmtsCallFlow" );
        pMsgParam = pMsgInfo->FirstChildElement("Parameter" );
        while ( pMsgParam )
        {
          int ID = atoi(pMsgParam->Attribute("ID"));
          switch(ID)
          {
            case 1:
            default:
            cout << "Parameter: name="  << ID << endl;
          }
          cout <<pMsgParam->Attribute("msgType") << endl;
          // cout <<pMsgParam->Attribute("msgBytes") << endl;
          cout <<pMsgParam->Attribute("processing") << endl;
          pMsgParam = pMsgParam->NextSiblingElement( "Parameter" );
         msgBytes = pMsgParam->Attribute("msgBytes");
#if 1
          unsigned char userData[1024];
          memcpy(userData, msgBytes.c_str(), msgBytes.length());
          printf("message bytes length %Zu\n", msgBytes.length());
          uint32_t _userDataLen = 0;
          char *pData = NULL;
          pData = cli_trim_line((char *)userData);
          _userDataLen = strlen(pData)/2;

          for (uint32_t i=0; i<(_userDataLen)*2; i+=2)
          {
            char msbChar;
            char lsbChar;
            msbChar = userData[i];
            lsbChar = userData[i+1];

            char byteStr[3];
            byteStr[0] = msbChar;
            byteStr[1] = lsbChar;
            byteStr[2] = '\0';
            userData[i/2] = (unsigned char)strtol(byteStr, NULL, 16);
          }
          PrintHexDump(userData, _userDataLen);
#endif
        }
#if 0
        // Parse parameters
        pParm1 = pRoot->FirstChildElement("Parameter1");
        msgType  = pParm1->Attribute("msgType");
        cout<<msgType <<endl;

        pParm2 = pRoot->FirstChildElement("Parameter2");
        msgBytes = pParm2->Attribute("msgBytes");
        cout<<msgBytes <<"\n length of msg is" <<msgBytes.length() <<endl;

        pParm3 = pRoot->FirstChildElement("Parameter3");
        procesInfo = pParm3->Attribute("processing");
        cout<<procesInfo <<endl;
#endif
      }
      else
      {
        cout << "Cannot find 'Configuration' node" << endl;
        return;
      }
    }
    else
    {
        cout << "Cannot load file"  << endl;
        return;
    }
}

int main(void)
{
    parseXML();   
    printf("i am here/n");
    return 0;
}

