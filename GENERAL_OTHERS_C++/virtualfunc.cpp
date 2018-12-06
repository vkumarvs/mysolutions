#include<iostream>
#include<stdio.h>
using namespace std;

class a
{
public:
    a()
    {
     // printf("base class constructor called\n");
    }
   ~a()
    {
      //printf("base class destructor\n");
    }

    virtual void  myfunc();
    void  myfunccall()
    {
      myfunc();
    }

};

#if 1
void a::myfunc()
{
    printf("default definition\n");
}
#endif
class b : public a
{
public:
    b()
    {
      //printf("class b constructor called\n");
    }
    ~b()
    {
     // printf("class b destructor\n");
    }
    void  myfunc();
};

void b::myfunc()
{
    printf("I am in class b\n");
}

int main()
{
    b p;
    p.myfunc();
    return 0;
}
