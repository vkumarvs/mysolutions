#include <stdio.h>
#include <vector>
#include <queue>

using namespace std;
template <typename T>
class Pool
{
public:
    Pool()
    {
      queue<T> x;
      queue<T> y;
      freeVector.push_back(x);
      freeVector.push_back(y);
      freeVector[0].push(10);
      freeVector[1].push(20);
      printf("size %Zu -- element %u element2 %u \n", freeVector.size(),
             freeVector[0].front(), freeVector[1].front());
    }
    ~Pool(){}
public:
    T x;
    vector <queue<T>  > freeVector;
    //vector <queue<int>  > freeVector();
};

int main()
{
    std::vector<int> first;
    first.push_back(10);
      //printf("size %Zu \n", first.size());

    Pool<int> poolObj;
#if 0
    queue<int> x;
    poolObj.freeVector.push_back(x);
      printf("size %Zu \n", poolObj.freeVector.size());
#endif
    //poolObj.freeVector(0).push(10);
    //queue<int> freeQ;
   // vector < queue<int> > freeVector(2);
    vector < int > arr(10);

    for(int x = 0;  x < 2; x++)
    {
     // freeVector.at(x).push(x);
      arr[x] = x;
    }
    printf("arr[0] = %u --- arr[1] == %u \n", arr[0], arr[1]);
#if 0
    // printf("value %u\n", freeVector[0].front());
   // freeQ.push(10);
     // printf("value %u\n", freeQ.front());
    for(int x = 0;  x < 10; x++)
    {
     freeVector.at(0).push(x);
    }

     // printf("value %u\n", freeVector[0]->front());
#if 1
    while(!freeVector.at(0).empty())
    {
      printf("value %u\n", freeVector.at(0).front());
      freeVector.at(0).pop();
    }
#endif
#endif
    return 0;
}
