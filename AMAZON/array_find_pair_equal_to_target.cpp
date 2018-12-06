#include <unordered_map>
#include <iostream>
#include <stdio.h>
using namespace std;

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
       // break;
      }
      else
      {
        std::pair<int, int> myshopping ((target-numbers[i]), i);
        map.insert(myshopping);
      }
    }

    return result;
}

int main()
{
    //int target  = 12;
    int target  = 9;
    //int array1[] = {1,4,6,8,9,11,13,16};
    int array1[] = {3,34,4,12,5,2};
    int *res = twoSum(array1, target, sizeof(array1)/sizeof(int));
    printf("pair is %d--%d\n", res[0], res[1]);
    return 0;
}
