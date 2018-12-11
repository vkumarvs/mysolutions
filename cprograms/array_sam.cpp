#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
using namespace std;
int main()
{
    unsigned char arr[50];
    arr[20] = 10;
    const char *x = "i love you";
    //unsigned char *ip = arr + 20;
    unsigned char *ip = &arr[20];
    memcpy(ip, x, 11);
    printf("value of ip is %s \n", ip);
    return 0;
}
