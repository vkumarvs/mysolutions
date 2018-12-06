#include <iostream>
#include <stdio.h>
#include <algorithm>

#include <unordered_map>
using namespace std;
#if 0
int res[] pairIndex(int arr[], int length)
{
  unordered_map<>
}
#else
//O(n) solution : unordered map is equal to hash map 
//it has average constant time insert and find
int *twoSum(int *numbers, int target, int length) 
{
    unordered_map<int, int> map;
    unordered_map<int,int>::iterator it;

    int *result = new int[2];

    for (int i = 0; i < length; i++) 
    {
      it = map.find(numbers[i]);
        if (it != map.end())
        {
          int index = it->second;
          result[0] = index ;
          result[1] = i;
          printf("pair is %d--%d\n", result[0], result[1]);
          //if u want to print all the pairs then dont use break otherwise for first pair use break
          break;
        }
        else
        {
         std::pair<int, int> myshopping ((target-numbers[i]), i);
          map.insert(myshopping);
        }
    }

    return result;
}
#endif

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
    //int array1[] = {1,2,5, 6, 4, 9, 8, 22, 13, 3, 7};
    int array1[] = {1,4,6,8,9,11,13,16};
    //int array1[] = {1,6,8,9,11,13,16};
    //int array1[] = {8,6,3,10,5,10};
 //   solution1(array1, number, sizeof(array1)/sizeof(int)); //O(n) solution
    //solution2(array1, number, sizeof(array1)/sizeof(int));
    int *res = twoSum(array1, 12, sizeof(array1)/sizeof(int));
    printf("pair is %d--%d\n", res[0], res[1]);
    return 0;
}

