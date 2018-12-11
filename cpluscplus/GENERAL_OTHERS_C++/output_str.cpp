#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
using namespace std;

int main()
{
    /*Get Imei String */
     string imeiSvStr;
    /*Get Imsi string */
    ostringstream outImsiStr;
    outImsiStr << 01 ;
    outImsiStr << 22 ;
    //imeiSvStr.assign();
    printf("output is %s\n", outImsiStr.str().c_str());
    return 0;
}
