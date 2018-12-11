#include <lib_func.hpp>

#if defined VIPIN
#include "DefaultClass.hpp"
extern DefaultClass myclass;
extern int globalMain;
#endif

uint32_t SigMger::GetB()
{
    return b;
}

bool SigMger::isFalseFlag()
{
    return isFalse;
}

#if defined VIPIN
uint32_t SigMger::GetA()
{
    printf("global value is %d myclass valur id %d\n", globalMain,
           myclass.GetT());
    return a;
}
#endif


