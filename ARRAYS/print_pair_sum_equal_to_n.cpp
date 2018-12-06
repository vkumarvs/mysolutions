#include <iostream>
#include <stdio.h>
#include <algorithm>
using namespace std;

int compare(const void *a, const void *b)
{
  int x = *((int *)a);
  int y = *((int *)b);
  return (x-y);
}


void solution3(int array1[], int number, int arrLength)
{
    qsort (array1, arrLength, sizeof(int), compare);
    for(int j = 0; j < arrLength; j++)
    {
      {
        cout<<array1[j]<<"\t";
      }
    }
    cout<<endl;

    for(int i = 0; i< arrLength; i++)
    {
        if(array1[i] >= number)
          break;
      for(int j = 0; j < arrLength; j++)
      {
        if((i!=j) && ((array1[j] != number) || (array1[i] != number)) &&
           ((array1[i] + array1[j]) == number))
        {
          cout<<"pair is " <<array1[i]<<"---" <<array1[j] <<endl;
        }
      }
    }

}

#include <stdlib.h>
#include <string.h>
void solution1(int array1[], int number, int arrLength)
{
    int newArr[50];
    //memset(newArr, 0, (sizeof(newArr)/sizeof(int)));
    for(int x = 0;  x < 50; x++)
    {
      newArr[x] = 0;
    }

    for(int x = 0;  x < arrLength; x++) //O(n) loop
    {
      newArr[array1[x]] = array1[x];
    }

    for(int x = 1; x < number ; x++)
    {
      //if((x <= (number-x)) && (x != (number -x)))
      if((x < (number-x)))
      {
        if((newArr[x] !=0) && (newArr[number-x] !=0))
          cout<<"pair is " <<newArr[x]<<"---" <<newArr[number - x] <<endl;
      }
    }
}

void solution2(int array1[], int number, int arrLength) //--- better than o(n^2)
{
    
    for(int i = arrLength -1; i >= 0; --i)
    {
      int start = 0;
      int end = i;
      while(start < end)
      {
#if 0
        if((array1[end] >= number)) 
        {
          printf("arr[end] %d number is greater than comp number \n", array1[end]);
          break;
        }
        if((array1[start] >= number))
        {
          printf("arr[start] %d number is greater than comp number \n", array1[start]);
          start++;
          break;
        }
#endif

        if((array1[start] + array1[end]) == number)
        {
          cout<<"pair is " <<array1[start]<<"---" <<array1[end] <<endl;
          break;
        }
        start++;
      }
    }

}

int main()
{
    int number = 10;
    //int number = 5;
    int array1[] = {1,2,5, 6, 4, 9, 8, 22, 13, 3, 7};
    //int array1[] = {8,6,3,10,5,10};
    solution1(array1, number, sizeof(array1)/sizeof(int)); //O(n) solution
    //solution2(array1, number, sizeof(array1)/sizeof(int));
    return 0;
}

