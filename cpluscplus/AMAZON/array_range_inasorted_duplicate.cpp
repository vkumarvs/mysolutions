#include <stdio.h>
/*
Given a sorted array of integers, find the starting and ending position of a given target value.
Your algorithm's runtime complexity must be in the order of O(log n).
If the target is not found in the array, return [-1, -1].

For example,
Given [5, 7, 7, 8, 8, 10] and target value 8,
return [3, 4].
*/

using namespace std;

int searchfirst(int *arr, int start, int end, int target, int length)
{
      int mid = (start + end)/2;
      if((target == arr[mid]) && ((mid == 0) || target > arr[mid-1]))
      {
        return mid;
      }

      if(target < arr[mid])
      {
        searchfirst(arr,start, mid-1, target,length);
      }
      else
      {
        searchfirst(arr,mid+1, end, target, length);
      }
}

int searchlast(int *arr, int start, int end, int target, int length)
{
      int mid = (start + end)/2;
      if((target == arr[mid]) && ((mid == length-1) || target < arr[mid+1]))
      {
        return mid;
      }

      if(target < arr[mid])
      {
        searchlast(arr,start, mid-1, target,length);
      }
      else
      {
        searchlast(arr,mid+1, end, target, length);
      }
}


int main()
{
   int arr[] = {1,3,5,6,7,7,7,7,10};
   int target = 7;
   int end = 0,start = 0;
   int length = sizeof(arr)/sizeof(int);
   start = searchfirst(arr, 0, length-1, target, length);
   //if(start != -1)
   end= searchlast(arr, start, length-1, target, length);
   printf("position of the element [%d %d] \n", start, end );
    return 0;
}
