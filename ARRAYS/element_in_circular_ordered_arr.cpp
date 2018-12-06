#include <iostream>
#include <stdlib.h>
#include <vector>
using namespace std;

bool isElementPresent(vector<int> &myArr, int elem, int start, int end)
{
    if(start > end)
    {
      cout<<"elementt not found---"<<endl;
      return false;
    }

    int pivot = (start+end)/2;
    cout<<"pivot is --- " <<pivot<<"start--"<<start<<"end --"<<end<<endl;
    if(myArr[pivot] == elem)
    {
      cout<<"element is found at index-----"<<pivot<<endl;
      return true;
    }

    cout<<"pivot val  --- " <<myArr[pivot]<<"start val--"<<myArr[start]<<"end val--"<<myArr[end]<<endl;

    if(elem < myArr[pivot] && elem >= myArr[start])
    {
      cout<<"3:element is in right side of pivot --start--"<<(pivot+1)<< "end--"<<end<<endl;
      isElementPresent(myArr, elem, start, pivot);
    }
    else if(elem < myArr[pivot] && elem < myArr[start])
    {
      cout<<"1:element is opposite side of pivot --start--"<<(pivot+1)<< "end--"<<end<<endl;
      isElementPresent(myArr, elem, pivot+1, end);
    }
    else if(elem > myArr[pivot] && elem <= myArr[end])
    {
      cout<<"4:element is in right side of pivot --start--"<<(pivot+1)<< "end--"<<end<<endl;
      isElementPresent(myArr, elem, pivot+1, end);
    }
    else if(elem > myArr[pivot] && elem > myArr[end])
    {
      isElementPresent(myArr, elem, start, pivot);
      cout<<"2:element is opposite side of pivot --start--"<<(pivot+1)<< "end--"<<end<<endl;
    }
    else
    {
      cout<<"invalid sorted array --- "<<endl;
      return false;
    }
}

int main()
{
    int arr[] = {5,6,8,10,1,2,4}; //circular sorted array
    //int arr[] = {1,2,4,5,6,8,10}; //non circular sorted array
    vector<int> myArr;
    for(int i = 0; i < 7; i++)
    {
       myArr.push_back(arr[i]);
    }
    int elem = 4;
    cout<<"is element present --- "<<(isElementPresent(myArr, elem, 0, 6)?"true":"false")<<endl;
    return 0;
}
