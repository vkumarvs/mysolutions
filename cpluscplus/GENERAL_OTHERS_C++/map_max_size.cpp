// map::max_size
#include <iostream>
#include <map>
#include<stdint.h>
#include <stdio.h>
using namespace std;

typedef pair<uint32_t, uint32_t> RanapPcPairType;
typedef map<RanapPcPairType, int > RanapPathMapPcPairType;


RanapPathMapPcPairType m_RncSgsnPathsPcPair;

bool GetPath(uint32_t opc, uint32_t dpc)
{
    RanapPcPairType   pcPair(opc, dpc);
    RanapPathMapPcPairType::iterator it;
    it = m_RncSgsnPathsPcPair.find(pcPair);
    if (it != m_RncSgsnPathsPcPair.end())
    {
      int x  = (*it).second;
      if(x == 100)
      {
        return true;
      }
    }
    else
    {
      return false;
    }
}

int main ()
{
    uint32_t opc = 121;
    uint32_t dpc = 131;
    RanapPcPairType   pcPair(opc, dpc);
    pair <RanapPathMapPcPairType::iterator, bool> retPcPair;
    retPcPair =
      m_RncSgsnPathsPcPair.insert(RanapPathMapPcPairType::value_type(pcPair, 100));
    if(retPcPair.second == true)
    {
      printf("successful \n");
    }
    else
    {
      printf("failed \n");
    }

    if(GetPath(opc, dpc))
    {
      printf("path earch successful\n");
    }
    else
    {
      printf("path earch failed\n");
    }

    return 0;
}
