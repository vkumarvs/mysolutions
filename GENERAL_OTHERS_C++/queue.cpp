// queue::push/pop
#include <iostream>
#include <queue>
using namespace std;

int main ()
{
    queue<int *> myqueue;
    myqueue.empty();
    int arr[5] = {1,2,3,4,5};
    int *p;
    for(int x = 0; x < 5; x++)
    {
      myqueue.push (&(arr[x]));
    }

    for(int y = 0; y < 8; y++)
    {
      p = myqueue.front();
      if(p)
      {
        cout << " " << *p <<endl;
      }
      else
{
cout <<"no data" <<endl;
}
      myqueue.pop();
    }
    return 0;
}
