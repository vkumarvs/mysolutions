#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

struct Example
{
    Example()
    {
      po = 0;
      lx = 0;
    }
    int po ;
    int lx ;
};

bool usingdoublePointer(int x, Example **ex);
bool usingsinglePointer(int x, Example *ex);
int main()
{
    int x = 20;
    Example *ex  = new Example();
    ex->po = 100;

    usingdoublePointer(x, &ex);
    printf("value of po %u and px %u\n", ex->po, ex->lx);
    usingsinglePointer(x, ex);
    printf("value of po %u and px %u\n", ex->po, ex->lx);
    return 0;
}

bool usingdoublePointer(int x, Example **ex)
{
    Example *tx = *ex;
    tx->lx = 300;
}

bool usingsinglePointer(int x, Example *ex)
{
    ex->lx = 200;
}

