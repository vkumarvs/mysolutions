#include <stdio.h>
#include <iostream>

int main() {
  int x = 0;
  int y = 10;
  for(x=0; x < 10; x++)
  {
      while (y > 0)
      {
          if(y==5)
          {
              goto printword;
          }
          y--;
      }
  }
printword:
  printf("value of y has reached 5\n");
 return 0;
}
