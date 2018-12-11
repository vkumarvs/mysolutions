#include <stdio.h>
#include <iostream>
using namespace std;
/*

fibonacci series is 0,1,1 2 3 5 8 13

printing odd 1 3 5 7 9 11 13 15 17 19 21

factorial 5*4*3*2*1

print a prime factors of a number
*/

int fibonacciRecur(int num)
{
  if(num <= 1)
    return num;
  else
    return (fibonacciRecur(num-1) + fibonacciRecur(num-2));
}

int fibonacciIterative(int num)
{
    int i = 0;
    int x_1 = 0, x_2 = 0;
    while(i < 10)
    {
      int tmp;
      if(i == 1)
      {
        x_1 = i;
        printf("%u \n", i);
      }
      else
      {
        tmp = x_1;     
        x_1 = x_1 + x_2;
        x_2 = tmp;
        printf("%u \n", x_1);
      }
     i++;
    }
        printf("\n");
}

int factorial(unsigned num)
{
    if(num == 0)
      return 1;
    else
      return num * factorial(num - 1);
}

void printOdd()
{
   for(int x = 1; x <= 99 ; x++)
   {
     if(x%2 != 0)
       printf("%d \t", x);
   }
   cout<<endl;
}

int main()
{
    /*Print first 10 elements*/
    for(int i = 0; i < 10; i++ )
    {
       printf("%d\n", fibonacciRecur(i));
    }
    printf("iterative solution\n");
    fibonacciIterative(10);
   // printf("factorial for 5 is %d  0:%d and 1:%d \n", factorial(5), factorial(0),factorial(1));
    //printOdd();
    return 0;
}
