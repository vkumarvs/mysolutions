#include <stdio.h>
#include <vector>
using namespace std;
int removeDuplicate(int *arr, int length)
{
    if ((0 == length) || (1 == length))
      return length;
   int i = 1;
    for (int x =1; x< length; x++)
    {
      if(arr[x] != arr[x-1])
      {
          arr[i++] = arr[x];
      }
    }
    if(i < length)
    arr[i] = '\0';
    return i;
}

//Little twist in the scene is instead removing duplicate in place remove the all instances of target value
int removeAllInstance(int *arr, int length, int val)
{
    if ((0 == length))
      return length;
   int i = 0;
    for (int x =1; x< length; x++)
    {
      if(arr[x] != val)
      {
          arr[i++] = arr[x];
      }
    }
    if(i < length)
    arr[i] = '\0';
    return i;
}

//More  twist in the scene is return the range index of a duplicate targated value
//Complexity must be o(logn) since it is sorted array
//For example, Given [5, 7, 7, 8, 8, 10] and target value 8, return [3, 4]
vector<int> returnRangeIndexOftarget(int *arr, int length, int val)
{
 vector<int> x;
#if 0
    if ((0 == length))
      return length;
   int i = 0;
    for (int x =1; x< length; x++)
    {
      if(arr[x] != val)
      {
          arr[i++] = arr[x];
      }
    }
    if(i < length)
    arr[i] = '\0';
    return i;
#endif
 return x;
}

void printarray(int *arr, int length)
{
    for(int x =0; x < length; x++)
    {
      printf("%d\t", arr[x]);
    }
    printf("\n");
}

int main()
{
    //int arr[] = {1,2,2,3,4,4,5,6,7,8};
    //int arr[] = {1,1,1,3,4,4,5,6,7,8};
    int arr[] = {7,8,9,1,1,1,4,5,6,6,6,7,8};
    int length = sizeof(arr)/sizeof(int);
    printarray(arr,length);
#if 0
    length = removeDuplicate(arr,length);
    printarray(arr,length);
#else
    length = removeAllInstance(arr,length,1);
    printarray(arr,length);
#endif
    return 0;
}
