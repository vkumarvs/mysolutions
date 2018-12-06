#include<stdio.h>
#define bool int

/* Function to check if x is power of 2*/
bool isPowerOfTwo (int x)
{
  /* First x in the below expression is for the case when x is 0 */
  //return x && (!(x&(x-1)));
  //return (!(x&(x-1)));
  //int y = 13;
  //return !y;
   return !x&(x-1);
}

/*Driver program to test above function*/
int main()
{
    int test_no = 0;
    if(isPowerOfTwo(test_no))
       printf("%d is a power of 2 %d", test_no, isPowerOfTwo(test_no));
    else
       printf("%d is not a power of 2 %d", test_no, isPowerOfTwo(test_no));
    getchar();
}

