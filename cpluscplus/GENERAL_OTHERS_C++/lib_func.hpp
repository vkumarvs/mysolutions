#include <iostream>
#include<stdio.h>
#include<stdint.h>

class SigMger
{
public:
    SigMger()
    {
      isFalse = true;
#if defined VIPIN
      a = 10;
#endif
      b = 20;
    }
    ~SigMger(){}
    bool isFalseFlag();
    uint32_t GetB();
#if defined VIPIN
    uint32_t GetA();
#endif
private:
    bool isFalse;
#if defined VIPIN
    uint32_t a;
#endif
    uint32_t b;
};

