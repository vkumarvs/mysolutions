#include<iostream>
#include<stdio.h>
using namespace std;

class a
{
public:
    a()
    {
      printf("base class constructor called\n");
    }
#if 0
   virtual ~a()
#else
   ~a()
#endif
    {
      printf("base class destructor\n");
    }

};

class b : public a
{
#if 1
public:
#else
private:
#endif
    b()
    {
      printf("class b constructor called\n");
    }
    ~b()
    {
      printf("class b destructor\n");
    }
};

int main()
{
 a *x;
 b *b_class = new b();
 x = b_class;
 delete x;
 //delete b_class;
return 0;
}
