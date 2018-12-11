#include <stdio.h>

int list[10] = {11, -12, 15, -3, 8, -9, 1, 8, 10, -2};

int main()
{
    int i,j;
    int maxSum = 0, sum;
    sum = 0;
    for(i=0; i<10; i++)
    {
      sum=0;
      for(j=i; j<10 ;j++)
      {
        sum = sum + list[j];
        maxSum = (maxSum>sum)?maxSum:sum;
        printf("jth(%d) s(%d) m(%d)-", list[j], sum, maxSum);
      }
    printf("\n\n\n");
    }

    printf("\nmaxSum = [%d]\n", maxSum);
    return 0;
}
