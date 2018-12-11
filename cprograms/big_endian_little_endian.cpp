#include <stdio.h>
#include <iostream>
using namespace std;

union endian
{
  int x ;
  char y;
};

int main()
{
  endian endx;
  endx.x = 1;
  if(endx.y == 1)
   printf("little endian\n");
  else
   printf("big endian\n");
return 0;
}
