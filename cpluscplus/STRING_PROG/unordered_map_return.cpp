#include <unordered_map>
#include <iostream>
#include <bitset>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

#if 0
vector<int> intersactionUsingHash(int *arr1, int *arr2, int size1, int size2)
{
    vector<int> newArr;
    unordered_map<int, int> myhash;
    for(int x = 0 ; x < size1; x++)
    {
      myhash[arr1[x]] = true;
    }

    for(int y = 0; y < size2; y++)
    {
      if(myhash[arr2[y]] == true)
         newArr.push_back(arr2[y]);
    }
    return newArr;
}
#else

vector<int> intersactionUsingHash(int *arr1, int *arr2, int size1, int size2)
{
    vector<int> newArr;
    unordered_map<int, int> myhash;
    for(int x = 0 ; x < size1; x++)
    {
      auto p = myhash.insert(make_pair(arr1[x], true));
      if(p.second)
      {
        cout<<"insertion successful"<<endl;
      }
    }

    for(int y = 0; y < size2; y++)
    {
      if(myhash[arr2[y]] == true)
         newArr.push_back(arr2[y]);
    }
    return newArr;
}
#endif
vector<int> intersactionUsingBinarySearch(int *arr1, int *arr2, int size1, int size2)
{
    vector<int> newArr;
    vector<int> newVect;
    for(int x = 0; x < size1; x++)
    {
      newVect.push_back(arr1[x]);
    }
    /*sort the array or make the binary tree */
    sort(newVect.begin(), newVect.end());
    cout<<"After sorting the array"<<endl;
    for(int x = 0; x < size1; x++)
    {
      cout<<newVect[x]<<"\t";
    }
    for(int j = 0; j < size2; j++)
    {
      if(binary_search(newVect.begin(), newVect.end(), arr2[j]))
      {
        newArr.push_back(arr2[j]);
      }
    }
    
    return newArr;
}
int main()
{
    int arr1[] = {5,2,1,4,3};
    int arr2[] = {4,5,8,10,11,12};
    int size1 = sizeof(arr1)/sizeof(int);
    int size2 = sizeof(arr2)/sizeof(int);

    vector<int> section = intersactionUsingBinarySearch(arr1,arr2,size1,size2);
    cout<<"intersaction elements using binary search are"<<endl;
    for(int x = 0; x < section.size(); x++)
    {
      cout<<section[x]<<"\t";
    }
    cout<<endl;

    vector<int> intersaction = intersactionUsingHash(arr1,arr2,size1,size2);
    cout<<"intersaction elements are"<<endl;
    for(int x = 0; x < intersaction.size(); x++)
    {
      cout<<intersaction[x]<<"\t";
    }
    cout<<endl;

    return 0;
}
