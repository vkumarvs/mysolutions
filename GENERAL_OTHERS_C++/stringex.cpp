#include<iostream>
#include<stdlib.h>
#include "stdio.h"
#include "string.h"
#include "stdint.h"
using namespace std;
int main()
{
    //char a[100];
    uint8_t a[100];
    char b[100];
    memcpy(a,"hi i am vipin",sizeof("hi i am vipin"));
    printf("value of a is %s\n",a);
    std::string x = (char *)a;
    printf("value of string is %s\n",x.c_str());
    const char *xt = x.c_str();
    int length = x.length();
    printf("string length is (%d)\n",length);
    memcpy(b,xt,sizeof(length));
    printf("value of copy string is %s\n",xt);
    return 0;
}
