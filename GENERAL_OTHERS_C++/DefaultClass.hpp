#include <iostream>
#include <stdio.h>
#include <stdint.h>
class DefaultClass
{
public :
    DefaultClass()
    {
      t = 1;
      p = 2;
    }
    int GetT()
    {
      return t;
    }
private:
    int t;
    int p;
};
