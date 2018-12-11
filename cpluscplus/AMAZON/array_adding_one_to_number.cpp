#include <stdio.h>
using namespace std;

void plusoneToNumber(int *digits, int length)
{
    printf("size of the array is %d \n", length);
    int *result = new int[length];
    int carry = 1;
    int val = 0;
    for(int i = (length - 1); i >= 0; i--)
    {
      val = digits[i] + carry;
      result[i] = val%10;
      carry = val/10;
      printf("index (%d) -- %d \t", i, result[i]);
    }
      printf(" \n");
    printf("new digits are \n");
    if(carry == 1)
    {
    //You dont need to copy rest of the digits except carry if u have to increase size
    //By default array is initialized with zero so putting carry should be enough
     printf("hello\n");
      result = new int[length + 1];
      result[0] = carry;
      for (int x = 0; x < length + 1; x++)
        printf("%d\t", result[x]);
    }
    else
    {
     printf("1:hello\n");
      for (int x = 0; x < length; x++)
        printf("%d\t", result[x]);
    }
      printf(" \n");
}


int main()
{
   int arr[] = {9,9,9};
   plusoneToNumber(arr, sizeof(arr)/sizeof(int));
    return 0;
}
