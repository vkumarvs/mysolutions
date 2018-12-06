#include <iostream>
#include <stdio.h>
using namespace std;

int main()
{
  int x = 10;
  bool amOk = true;
  bool saveVal;
  //saveVal = __sync_fetch_and_and(&amOk, false);
  __sync_fetch_and_or(&amOk, true);
  __sync_fetch_and_add(&x, 10);
  printf("amOk previous val %u -- new val %u -- x val %u \n", saveVal, amOk, x);
  return 0;
}
