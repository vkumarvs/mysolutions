#include <iostream>
#include <stdint.h>
#include<stdio.h>

using namespace std;

/*
 *Non-const and const reference binding follow different rules

These are the rules of the C++ language:

    an expression consisting of a literal number (12) is a "rvalue"
    it is not permitted to create a non-const reference with a rvalue: int &ri = 12; is ill-formed
    it is permitted to create a const reference with a rvalue: in this case, an unnamed object is created by the compiler; this object will persist as long as the reference itself exist.

 * */


int GetA()
{
    int p = 20;
    return p;
}

void SetB(const int &x)
{
    printf("B is %d\n", x);

}


void SetC(int x)
{
    printf("C is %d\n", x);

}

void SetD(int &x)
{
    printf("B is %d\n", x);

}

int main()
{
    int y = 10;
#if 0
    SetB(GetA());
    
    SetC(GetA());

    SetD(y);
#else
    /*Problem while calling D */
    SetD(GetA());
#endif

    return 0;
}
