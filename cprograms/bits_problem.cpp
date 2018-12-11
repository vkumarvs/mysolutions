#include <stdio.h>
#include <iostream>
#include <math.h>
using namespace std;

/*
5 = 101 >>1 0101 & 0001

Describe a function that takes an int value, and returns true if the bit pattern of that
 int value is the same if you reverse it (i.e. it's a palindrome); i.e. boolean isPalindrome(int x)
for example bit pattern for 5 is in palindromic order
*/
int count_bits(unsigned y)
{
    int bit_count = 0;
    while(y!=0)
    {
      if(y & 1)
      {
        bit_count++;
      }
      y=y>>1;
    }
  printf("number of bits are %d\n", bit_count);
}

bool bit_palindrome(int x)
{
    int result = 0;
    int number = x;
    while(x)
    {
      if(x & 1)
      {
        result = ((result << 1) | 1);
      }
      else
      {
        result = ((result << 1));
      }
      x = x >> 1;
    }

    if(result == number)
    {
      printf("number bits are in palindrome order\n");
      return true;
    }
    else
    {
      printf("number bits are not in palindrome order\n");
      return false;
    }
}

int convert_number(int number, int in_base, int out_base)
{
    int origNumber = number;
    int pos = 0;
    int convert_number = 0;
    while(number !=0)
    {
      int mod = number % out_base;
      convert_number+=mod*pow(in_base, pos); 
      number = number / out_base;
      pos++;
    }
    if((out_base == 16))
{
}
    printf("converted number(%d) from base (%d) to base (%d) --- result(%d)\n", origNumber, in_base, out_base, convert_number);
}

int main()
{
    //count_bits(15);
    //bit_palindrome(5);
    //bit_palindrome(6);
    //convert_number(0x10, 16, 10);
    convert_number(10, 16, 10);
    convert_number(16, 10, 16);
    convert_number(22, 10, 16);
    convert_number(11, 10, 16);
      return 0;
}


