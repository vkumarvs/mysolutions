#include<stdio.h>

int add(int a, int b)
{
  if(!a)
    return b;
  else
   return add((a&b)<<1,a^b);
}
int main()
 {
   int a = 7,b=17;
   char s=0;
   while(s+=a&1,a>>=1);
   printf("%d\n",s);
   printf("%d\n",add(a,b));
 }
