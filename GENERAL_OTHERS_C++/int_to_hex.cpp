#include<stdio.h>
#include<stdint.h>

int main()
{
    //uint8_t gSLRFirstByte = 0, gSLRSecondByte = 0, gSLRThirdByte = 0;
    unsigned char gSLRFirstByte = 0, gSLRSecondByte = 0, gSLRThirdByte = 0;
    int x = 10;
    int y = 49;
    unsigned char userData[3];
    switch(x)
    {   
      case 10:
      //for(uint8_t first = gSLRFirstByte; first < 256;)
      for(unsigned char first = gSLRFirstByte; first < 256;)
      { 
        //for(uint8_t second = gSLRSecondByte; second < 256;)
        for(unsigned char second = gSLRSecondByte; second < 256;)
        {
          //for(uint8_t third = gSLRThirdByte; third < 256;)
          for(unsigned char third = gSLRThirdByte; third < 256;)
          {
            third++;
            gSLRThirdByte = third;
            break;
          }
          second++;
          gSLRSecondByte = second;
          break;
        }
        first++;
        gSLRFirstByte = first;
        break;
      }
      userData[1] = (unsigned char)gSLRFirstByte;
      userData[2] = (unsigned char)gSLRSecondByte;
      userData[3] = (unsigned char)gSLRThirdByte;
      printf("value of first(%x) second(%x) and third (%x)\n", userData[1], userData[2], userData[3]);
      break;
      default:
      {
        printf("invalid message type \n");
      }
    }

}
