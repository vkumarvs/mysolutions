#include <stdio.h>
#include <math.h>
#include <stdint.h>

int powerFun(uint32_t x, uint32_t pow)
{
    int  ret = 1;
    while(pow)
    {
       ret = ret*x;
       pow--;
       printf("loop1--- \n");
    }
    return ret;
}

/*idea is we can divide exp part into half and if module is not zero
* multiple it with base*/
int fastPower(int n, int exp)
{
    int halfExp = exp/2;
    int power = 1;
    for(int i=0; i<halfExp;i++, power=power*n);
    power = power*power;
    if(exp%2 > 0){
      power = power*n;
    }
    return power;
}

int ipow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1) //
            result *= base;
        exp >>= 1; //divide exp in half like 3*3*3*3*3 == (9)*9*3
        base *= base;
       printf("loop2--- \n");
    }

    return result;
}

int main()
{
   float a = 10.5; int b =2;
   double t = pow(10,2);
   //printf("pow of a and b  %lf\n",  pow(a,b));
 //  printf("pow of a and b  %u\n",  powerFun(10, 2));
   printf("pow of a and b  %u\n",  powerFun(24, 5));
   printf("pow of a and b  %u\n",  ipow(24, 5));
   return 0;
}
