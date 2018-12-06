#define __STDC_FORMAT_MACROS  
#include "inttypes.h"  
#include "stdio.h"

int main()
{
    uint64_t x = 1234567;
    uint64_t y = 1234567;
    printf("value of x %" PRIu64 " value of y is %" PRIu64 "\n", x, y);
    return 0;
}
