#include <stdio.h>
#include <iostream>

class A
{
public :
    int myfunc()
    {
      printf("i am here in A\n");
    }
    A(){}
    //~A()
    virtual ~A()
    {
      printf("i am in class A dest\n");
    }
};

class B:public A
{
public:
    B()
    {
      myfunc();
  //    B();
    }
    int myfunc()
    {
      printf("i am here in B\n");
    }
    ~B()
    {
      printf("i am in class B dest\n");
    }
};

int main()
{
    B *x = new B();
    A *parent = new B();
    delete parent;
    printf("calling B function\n");
    x->myfunc();
    delete x;
}
