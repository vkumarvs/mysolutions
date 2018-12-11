#include <iostream>
#include <stdio.h>
#include<stdlib.h>
#include <vector>

using namespace std;

void findPairSumTok(int array1[], int number, int arrLength);
void findAllTriplet(int array1[], int size);

int main()
{
    int arr[100];
    int size = 0;
    char ch;
    printf("OPTIONS: c: create \n p: print t: all triplet combination - two number sum equals to third element\n s-sum of subarray of size k equal to x \n e-print all subset \n q- quit");
    while(1)
    {
      printf("Enter option value\n");
      scanf("%c", &ch);
      switch(ch)
      {
        case 'c':
        int l;
        printf("Enter number of element to enter in array\n");
        scanf("%d", &size);
        for(int i = 0; i < size; i++)
        {
          printf("Enter element\n");
          {
            scanf("%d", &l);
            arr[i] = l;
          }
        }
        break;
        case 'p':
        for(int i =0; i < size; i++)
        {
          printf("%d\t", arr[i]);
        }
        printf("\n");
        break;
        case 't':
        findAllTriplet(arr, size);
        break;
        case 's':
        break;
        case 'e':
        break;
        case 'q':
        exit(0);
        break;
        default:
        printf("invalid operation \n");
      }
    }
    return 0;
}

void findAllTriplet(int arr[], int size)
{
  for(int i = 0; i < size; i++)
  {
     findPairSumTok(arr, arr[i], size);
  }
}

void findPairSumTok(int array1[], int number, int arrLength)
{
    vector<int> tmpArr(arrLength);
    for(int x = 0;  x < arrLength; x++) //O(n) loop
    {
      tmpArr[array1[x]] = array1[x];
    }

    for(int x = 1; x < number ; x++)
    {
      if((x < (number-x)))
      {
        if((tmpArr[x] !=0) && (tmpArr[number-x] !=0))
          cout<<"pair is " <<tmpArr[x]<<"," <<tmpArr[number - x] <<"," <<number <<endl;
      }
    }
}
