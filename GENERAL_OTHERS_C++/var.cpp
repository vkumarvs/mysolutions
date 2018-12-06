#include "iostream"
#include "stdio.h"
#include <arpa/inet.h>
void function_same_car(int *a, int* b)
{
    int p = ntohs(*a);
    int q = ntohs(*b);
    printf("value of p = %d -- %d\n", p, q);
}

int main()
{
    int x = 10;
    function_same_car(&x,&x);
    return 0;
}
