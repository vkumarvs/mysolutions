#include <iostream>
#include <stdio.h>
using namespace std;
class test1
{
public:
  test1()
  {
    x =10;
    y=20;
  }
  test1(int x1, int y1)
  {
    x = x1;
    y = y1;
  }
 ~test1()
 {
      printf("destroyed test1 object\n");
}
 int x;
 int y;
};

test1 *func1()
{
  test1 *test = new test1;
  printf("func1 returning obj pointer %p \n",test);
  return test;
}

int main()
{
    test1 *fun = NULL;
    test1 fun2(30,40);
    fun = func1();
    if(fun)
    {
      *fun = fun2;
      printf("fun contained following value %p %d %d \n",fun,fun->x,fun->y);
      //delete fun;
    }
    else
    {
      printf("fun contained NULL value \n");
    }
    return 0;
}
