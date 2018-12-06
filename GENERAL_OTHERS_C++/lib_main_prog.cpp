#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include "lib_func.hpp"
#include "DefaultClass.hpp"
using namespace std;
int globalMain = 20;

DefaultClass myclass;

int main()
{
   SigMger x;
   printf("b is \n",x.GetB());
#if defined VIPIN
   printf("a is \n",x.GetA());
#endif
   return 0;
}
