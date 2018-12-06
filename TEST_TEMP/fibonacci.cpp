#include <iostream>
#include <queue>
using namespace std;

int fbn(int x) {
    if(x==1 || x == 2)
          return 1;
   cout << "number x:" << x << endl;
   int y = fbn(x-1) + fbn(x-2);
   return y;
}



int main() {
  cout << fbn(8) <<endl;
  return 0;
}
