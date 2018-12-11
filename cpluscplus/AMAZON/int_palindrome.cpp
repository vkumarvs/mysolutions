#include <stdio.h>
#include <limits.h>
using namespace std;

int reverse(int num)
{
   if(num < 0)
   {
     return -1;
   }
   int rev = 0;
   while(num!=0)
   {
     rev = rev*10+num%10;
     num=num/10;
   }
   if(rev > INT_MAX || rev < INT_MIN)
     return -1;
   else
     return rev;
}
int main()
{
    int num = 123454321;
    int result = reverse(num);
    if(-1 != result && (num == result))
    {
      printf("Numbe ris palindrome %d\n", result);
    }
    else
    {
      printf("Number is not a palindrome %d\n", result);
    }
    return 0;
}
