#include <stdio.h>
/*
 * 9. Given a sorted array and a target value, return the index if the target is
 * found. If not, return the index where it would be if it were inserted in
 * order.
 *
 * You may assume no duplicates in the array.
 *
 * Here are few examples.
 * [1,3,5,6], 5 → 2
 * [1,3,5,6], 2 → 1
 * [1,3,5,6], 7 → 4
 * [1,3,5,6], 0 → 0
 */
using namespace std;

int searchinsert(int *arr, int start, int end, int target)
{
   int mid = start + (end - start)/2;
   if(target == arr[mid])
   {
       return mid;
   }

   if(target < arr[mid])
   {
    return start < mid?searchinsert(arr,start, mid-1, target):start;
   }
   else
   {
    return end > mid?searchinsert(arr,mid+1, end, target):end+1;
   }
}


int main()
{
   int arr[] = {1,3,5,6};
   int target = 7;
   int position = searchinsert(arr, 0, (sizeof(arr)/sizeof(int) - 1), target);
   printf("position of the element %d\n", position);
    return 0;
}
