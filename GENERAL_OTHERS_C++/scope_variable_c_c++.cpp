#if 0
#include <stdio.h>

int main()
{
    int x, y;

    printf("value of x and y :: %d--%d", x, y);

    int z = 10;
    printf("value of z is %d\n", z);

    return 0;
}
#else
#include <stdio.h>
int main()
{
    foo();
    return 0;
}

int foo()
{
    printf( "Hello world" );
}
#endif
