/*
 14. Write an efficient algorithm that searches for a value in an m x n matrix. This matrix has the following properties:

    Integers in each row are sorted from left to right.
    The first integer of each row is greater than the last integer of the previous row.

For example,

Consider the following matrix:

[
  [1,   3,  5,  7],
  [10, 11, 16, 20],
  [23, 30, 34, 50]
]

Given target = 3, return true

Efficient linear solution: works for all cases
1) Start from the top right element
2)If element is equal to then return
3)If element is less then target value move down
4)If element is greater then target value move left
5) Repeat 2,3 and 4

*/
#include<stdio.h>
//This function will work only for sorted matrix and whose last element in the
// int mat[3][4] = { {1,   3,  5,  7},
//                    {10, 11, 16, 20},
//                    {23, 30, 34, 50},
//row is smaller then first element of next row
int binarysearch(int mat[][4], int row, int column, int target)
{
   int low = 0, high = row*column-1;
   while(low != high)
   {
     int mid = (low + high)/2;
     int x = mid/column;
     int y = mid%column;
     int midelem = mat[x][y];
     printf("value of low(%d) and high(%d) mid(%d) -- element(%d)\n", low,high, mid, midelem);
     if(midelem  < target)
     {
       low = mid + 1;
     }
     else if(midelem  > target)
     {
       high = mid;
     }
     else if(midelem == target)
     {
       printf("element index is row--col (%d--%d) \n", mid/column, mid%column);
       break;
     }
     else
     {
       printf("invalid\n");
     }
   }
   return 0;  // if ( i==n || j== -1 )
}

 
/* Searches the element x in mat[][]. If the element is found,
    then prints its position and returns true, otherwise prints
    "not found" and returns false */
//int search(int mat[4][4], int n, int x)
//It has linear time complexity
int search(int mat[][4], int n, int x)
{
   int i = 0, j = n-1;  //set indexes for top right element
   while ( i < n && j >= 0 )
   {
      if ( mat[i][j] == x )
      {
         printf("\n Found at %d, %d", i, j);
         return 1;
      }
      if ( mat[i][j] > x )
        j--;
      else //  if mat[i][j] < x
        i++;
   }
 
   printf("\n Element not found");
   return 0;  // if ( i==n || j== -1 )
}
 
// driver program to test above function
int main()
{
#if 0
  int mat[4][4] = { {10, 20, 30, 40},
                    {15, 25, 35, 45},
                    {27, 29, 37, 48},
                    {32, 33, 39, 50},
                  };
#else
#if 1
  int mat[3][4] = { {1,   3,  5,  7},
                    {10, 11, 16, 20},
                    {23, 30, 34, 50},
  };
#else
 int mat[4][4] = { {1, 5, 7, 13},
                    {2, 6, 10, 15},
                    {3, 7, 11, 16},
                    {4, 8, 14, 17},
 };
#endif
#endif
  //search(mat, 4, 29);
  //search(mat, 3, 34);
 // search(mat, 4, 14);
  binarysearch(mat, 3, 4, 30);
  getchar();
  return 0;
}
