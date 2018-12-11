# include <stdio.h>
//# include <conio.h> 

//http://www.geeksforgeeks.org/write-a-c-program-to-print-all-permutations-of-a-given-string/

/* Function to swap values at two pointers */
void swap (char *x, char *y)
{
    char temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

/* Function to print permutations of string
   This function takes three parameters:
   1. String
   2. Starting index of the string
   3. Ending index of the string. */
void permute(char *a, int i, int n)
{
   int j;
   if (i == n)
     printf("permutation %s\n", a);
   else
   {
        for (j = i; j <= n; j++)
       {
          printf("values %d %d \n", i, j);
          swap((a+i), (a+j));
          printf("after swap %s \n", a);
          permute(a, i+1, n);
          swap((a+i), (a+j)); //backtrack
       }
   }
} 

/* Driver program to test above functions */
int main()
{
   //char a[] = "ABC";
   char a[] = "123";
   //char a[] = "122";
   permute(a, 0, 2);
   getchar();
   return 0;
}

