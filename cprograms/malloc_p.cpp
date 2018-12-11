#include <string.h>
#include <stdlib.h>
#include <stdio.h>
int main(){  
    int *p;
    unsigned int mb = 0;
    while(1){
        //p=(int *)malloc(1024*1024*1024);
        malloc(1024*1024*8);
        //if(!p)break;
        mb++;
        printf("mb %u \n", mb);
    }   
}
