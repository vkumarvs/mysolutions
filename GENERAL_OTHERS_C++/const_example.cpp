#include <stdio.h>
#include <iostream>
#include <stdint.h>
using namespace std;

class example
{
public:

    example(){}
    example(int a, int b)
    {
      A = a;
      B = b;
    }
    ~example(){}
    int & getA(){return A;}
    const int & getB(){return B;}
private:
    int A;
    int B;

};

int main()
{
    example x(10, 20);
    int &p = x.getA();
    p = 30;
    const int &t = x.getB();
    t = 40;
    printf("value of p(%d) and t(%d) \n", p, t);
    return 0;
}

