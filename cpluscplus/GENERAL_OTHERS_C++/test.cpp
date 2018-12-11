#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int main()
{
    char        data[32];
    memset(data, 0, 32);
    //data[1] = 20;
    if(data[0])
      printf("i am here \n");
    else
      printf("not valuid  \n");

    return  0;
}
