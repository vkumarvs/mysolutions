#include<stdio.h>
void * func1()
{
    int y;
    y = 10;
    return (void *)y;
}
int main()
{
    void *x = 0x00;
    x = func1();
    if(x == NULL)
    {
      printf("error\n");
    }
    else
    {
      printf("value of x (%d)\n",x);
    }
    return 0;
}
