#include <iostream>
#include <bitset>
using namespace std;

int main ()
{
  bitset<4> mybits;

  cout << mybits.set() << endl;       // 1111
  cout << mybits.set(2,0) << endl;    // 1011
  cout << mybits.set(2) << endl;      // 1111

  return 0;
}
