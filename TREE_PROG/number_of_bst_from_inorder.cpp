#include <iostream>
using namespace std;


int BSTs(int n)
{
    int total = 0;
    if ( n <= 1)
      return 1;
    for(int i = 1; i<=n; i++) {
      total+= BSTs(i-1)*BSTs(n-i);
    }
    return total;
}

int main()
{
    int node = 10;
    cout<<"Number of BST possible  ---- "<<BSTs(node) <<"From node --"<<node <<endl;
    return 0;
}
