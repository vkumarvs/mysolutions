#include <stdio.h>
#include <string.h>
int main()
{
    int newArr[50] = {0};
    int x = 0;
    memset(newArr, 0, 50*4);
    for(x = 0;  x < 50; x++)
    {
      printf("%d \t", newArr[x]);
    }
    printf("\n");

}
