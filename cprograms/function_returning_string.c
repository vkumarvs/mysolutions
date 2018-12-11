#include <stdio.h>

char * stringReturn()
{
  char *x = "ramp";
  return x;

  /*Below is wrong method: returning function should use some static buffer */
  //char p[10] = "ramp123";
  // return p;
}

int main()
{
    printf("string is %s\n", stringReturn());
}
