#include <iostream>
#include <stdio.h>

using namespace std;

int main()
{
    int y = 100;
    int newval = 90;
    int p = __sync_val_compare_and_swap(&y, y, newval);
    printf("value of y is (%d)\n", y);
    return 0;
}
