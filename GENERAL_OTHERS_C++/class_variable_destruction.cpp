#include<iostream>
#include<stdio.h>
using namespace std;

class a
{
public:
    a()
    {
      printf("class a constructor called\n");
    }
   ~a()
    {
      printf("class a destructor\n");
    }

};

class b 
{
private:
    a x;
public:
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
 b *temp = new b();

delete temp;
return 0;
}
