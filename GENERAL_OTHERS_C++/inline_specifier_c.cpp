#include<iostream>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string>

/*Function to measure number of CPU cycle*/
inline uint64_t rdtsc(uint32_t lo,uint32_t hi )
{
       asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
          return (uint64_t)hi << 32 | lo;
}

inline int foo(){
return 3;
}

void g() {
printf("foo called from g: return value = %d, address = %p\n", foo(), &foo);
}

int main() 
{
    double t;
    uint32_t lo=0, hi=0;
    t=rdtsc(lo,hi );

    printf("foo called from main: return value = %d, address = %p\n", foo(), &foo);
    g();
    double cycle;
    cycle=rdtsc(lo,hi ) - t;
    //cout<<"the value is :: "<<cycle<<endl;
    printf("value of cycle is %lf\n", cycle);
}

