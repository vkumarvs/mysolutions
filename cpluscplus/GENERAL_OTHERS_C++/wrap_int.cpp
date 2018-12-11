#include <stdio.h>
#include <iostream>
#include <stdint.h>
using namespace std;

int main()
{
    uint64_t m_TotalDroppedMsgs = 4294967298 ;
    uint32_t m_TestNo = 4294967295; //max uint32_t value
    //m_TotalDroppedMsgs = m_TestNo;
    printf("dropped Cnt %u   test Cnt %u \n ", (uint32_t)m_TotalDroppedMsgs, m_TestNo);
    return 0;
}

