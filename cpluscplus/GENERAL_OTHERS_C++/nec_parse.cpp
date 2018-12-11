#include <stdio.h>
#include <iostream>

#define RANAP_GET_INT32(pbuf, data) {                         \
      (data) = (*(pbuf) << 24) | (*((pbuf) + 1) << 16) |      \
               (*((pbuf) + 2) << 8) | (*((pbuf) + 3) );       \ 
}

void get_int_32(char *encodedBuf, )
{
}

int main()
{
uint32_t rnc_ip;
char buf1 = {,,,,};
char buf2 = {,,,,};
printf();
    return;
}
