#include <iostream>

#include <iostream>
#include <stdio.h>
#include <stdint.h>

enum
{       
    RANAP_PDP_STATE_IDLE= 0,
    RANAP_PDP_STATE_REQUESTED,
    RANAP_PDP_STATE_ESTABLISHED,
    RANAP_PDP_STATE_DEACT_REQUESTED
};        

const char* GetStateString()
{
    const char* stateString = NULL;
    int state = RANAP_PDP_STATE_ESTABLISHED;
    if (state == RANAP_PDP_STATE_ESTABLISHED)
      stateString = "ESTABLISHED_STATE ";
    else if (state == RANAP_PDP_STATE_REQUESTED)
      stateString = "REQUESTED_STATE";
    else if (state == RANAP_PDP_STATE_DEACT_REQUESTED)
      stateString = "DEACT_REQUESTED_STATE";
    else
      stateString = "INACTIVE_STATE";
    return stateString;
}

int main()
{
    printf("state is %s\n", GetStateString());
    return 0;
}
