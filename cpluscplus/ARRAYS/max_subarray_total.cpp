#include <stdio.h>

//int list[10] = {11, -12, 15, -3, 8, -9, 1, 8, 10, -2};
//int numbers[10] = {11, -12, 15, -3, 8, -9, 1, 8, 10, -2};
int list[7] = {14, -18, 5, 6, -7, 8, 20};
int numbers[7] = {14, -18, 5, 6, -7, 8, 20};
void max_subarray_total_no_sequence()
{
    int i,j;
    int maxSum = 0, sum;
    sum = 0;
    //for(i=0; i<10; i++)
    //for(i=0; i<7; i++)
    {
      sum=0;
      //for(j=i; j<10 ;j++)
      for(j=i; j<7 ;j++)
      {
        sum = sum + list[j];
        maxSum = (maxSum>sum)?maxSum:sum;
      }
    }

    printf("\nno sequence maxSum = [%d]\n", maxSum);
}

void max_subarray_total_sequence()
{
    int i,j;
    int maxSum = 0, sum;
    sum = 0;
    //for(i=0; i<10; i++)
    for(i=0; i<7; i++)
    {
      sum = sum + list[i];
      maxSum = (maxSum>sum)?maxSum:sum;
      if(sum < 0)
      {
        sum = 0;
      }
    }
    printf("\nsequence maxSum = [%d]\n", maxSum);
}

int sequence()
{
        // Initialize variables here
        int max_so_far  = numbers[0], max_ending_here = numbers[0];
        size_t begin = 0;
        size_t begin_temp = 0;
        size_t end = 0;
        // Find sequence by looping through
        //for(size_t i = 1; i < numbers.size(); i++)
        //for(size_t i = 1; i < 8; i++)
        //for(size_t i = 1; i < 10; i++)
        for(size_t i = 1; i < 7; i++)
        {
                // calculate max_ending_here
                max_ending_here += numbers[i];
                if(numbers[i] > max_ending_here)
                {
                        max_ending_here = numbers[i];
                        begin_temp = i;
                }
                // calculate max_so_far
                if(max_ending_here > max_so_far )
                {
                        max_so_far  = max_ending_here;
                        begin = begin_temp;
                        end = i;
                }
        }
        printf("sequence maxsum %u start %zu end %zu\n", max_so_far, begin, end);
        // return max_so_far
        return max_so_far ;
}

int main()
{
    max_subarray_total_no_sequence();
    max_subarray_total_sequence();
    sequence();
    return 0;
}
