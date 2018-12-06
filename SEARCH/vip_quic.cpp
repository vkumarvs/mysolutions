#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <vector>
using namespace std;

#define MAX_ARRAY_SIZE 10
#define CHAR_TYPE 0
#define INT_TYPE 1

void PrintArray(int *arr, int size)
{
    for(int x = 0; x < size; x++)
    {
      cout <<" " <<arr[x];
    }
   cout <<endl;
}

void PrintArray(char *arr , int size)
{
    for(int x = 0; x < size; x++)
    {
      cout <<" " <<arr[x];
    }
}

void swap(int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void swapChar(char *a, char *b)
{
  char tmp = *a;
  *a = *b;
  *b = tmp;
}

int partition(int *arr, int start, int end, int pivotIdx)
{
    int sortedIdx = start;
    swap(&arr[pivotIdx], &arr[end]);
    for(int x = start; x < (end); x++)
    {
      if(arr[x] < arr[end])
      {
        swap(&arr[sortedIdx], &arr[x]);
        sortedIdx++;  
      }
    }
    swap(&arr[sortedIdx], &arr[end]);
    //PrintArray(arr, 10);
    return sortedIdx;
}

int partitionChar(char *arr, int start, int end, int pivotIdx)
{
    int sortedIdx = start;
    swapChar(&arr[pivotIdx], &arr[end]);
    for(int x = start; x < (end); x++)
    {
      if(arr[x] < arr[end])
      {
        swapChar(&arr[sortedIdx], &arr[x]);
        sortedIdx++;  
      }
    }
    swapChar(&arr[sortedIdx], &arr[end]);
    //PrintArray(arr, 10);
    return sortedIdx;
}

void QuicSort(int *arr, int start, int end)
{
    if(end <= start) 
      return;
    int pivotIdx = start;
    pivotIdx = partition(arr, start, end, pivotIdx);
    QuicSort(arr, start, (pivotIdx - 1));
    QuicSort(arr, (pivotIdx + 1) , end);
}

void QuicCharSort(char *arr, int start, int end)
{
    if(end <= start) 
      return;
    int pivotIdx = start;
    pivotIdx = partitionChar(arr, start, end, pivotIdx);
    QuicCharSort(arr, start, (pivotIdx - 1));
    QuicCharSort(arr, (pivotIdx + 1) , end);
}


int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int main()
{
    int array[] = {11,2,9,13,57,25,17,1,90,3};
    //char str[] = "my name is vipin";
    char str[] = "mynameisvipin";
    vector<char> myvector (str, str+13);               // 32 71 12 45 26 80 53 33
    vector<char>::iterator it;
    sort (myvector.begin(), myvector.end());
    cout << "myvector contains:" ;
    for (it=myvector.begin(); it!=myvector.end(); ++it)
      cout << " " << *it;

    cout << endl;
#if 0
    int arr[MAX_ARRAY_SIZE];
    for(int x = 0; x < MAX_ARRAY_SIZE; x++)
    {
      cout <<"Enter number" <<endl;
      cin >> arr[x];
    }
#endif
    //cout <<"Your entered array is " <<endl;
    //PrintArray(array, 10);
    cout <<"Your entered char array is " <<endl;
    PrintArray(str, (int)sizeof(str));
    cout <<endl;
#if 0
    cout <<"Your sorted array is " <<endl;
    QuicSort(array, 0, 9);
    PrintArray(array, 10);
#else
    cout <<"Your sorted array is " <<endl;
    QuicCharSort(str, 0, ((int)sizeof(str) - 1));
    //qsort (str, sizeof(str), sizeof(char), compare);
    PrintArray(str, (int)sizeof(str));
    cout <<endl;
#endif

    cout <<"Your sorted array is " <<endl;
    //qsort (array, 10, sizeof(int), compare);
    qsort (array, 10, sizeof(int), compare);
    //sort();
    PrintArray(array, 10);
    cout <<endl;
    return 0;
}

