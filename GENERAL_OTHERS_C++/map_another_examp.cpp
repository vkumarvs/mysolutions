#include <iostream>
#include <map>
#include <string>
#include<list>
#include<stdint.h>
#include "stdio.h"
using namespace std;

int main()
{
    map <uint32_t , bool> Ip_Key_Int_Map;
    Ip_Key_Int_Map.insert(map <uint32_t , bool>:: value_type(1000 , false));

    map <uint32_t, bool>::iterator it;
    it = Ip_Key_Int_Map.find(1000);
    if (it != Ip_Key_Int_Map.end())
    {
      (*it).second = true;
    }

    map <uint32_t, bool>::iterator it1;
    it1 = Ip_Key_Int_Map.find(1000);
    if (it1 != Ip_Key_Int_Map.end())
    {
      printf("bool value is %d\n", (*it).second);
    }
    return 0;
}
