#include <iostream>
#include <map>
#include <stdio.h>
#include<stdint.h>

using namespace std;

struct PdpNetworkLocalKey
{
    PdpNetworkLocalKey(uint32_t a, bool b):ip(a), isLocal(b) { }

    PdpNetworkLocalKey():ip(0), isLocal(0) { }

    bool operator<(const PdpNetworkLocalKey& rhs) const {
      if(this->ip == rhs.ip)
      {
        return (this->isLocal < rhs.isLocal );
      }
      else
      {
        return (this->ip < rhs.ip);
      }
    }

    bool operator==(const PdpNetworkLocalKey& rhs) const {
      return((this->ip == rhs.ip) && (this->isLocal == rhs.isLocal));
    }

    uint32_t ip;
    bool isLocal;
};

#if 1
struct classcomp {
  bool operator() (const PdpNetworkLocalKey& lhs, const PdpNetworkLocalKey& rhs) const {
    if(lhs.ip == rhs.ip)
    {
      // Todo: Use lhs.isLocal and rhs.isLocal here to order the items as you like
     // printf("I am in equal ip lhs ip %u bool %d  rhs ip %u bool %d\n",
       //      lhs.ip, lhs.isLocal, rhs.ip, rhs.isLocal);
      return (rhs.isLocal > lhs.isLocal);
    }
    else
    {
      // Order by ip
 //     printf("I am in different ip lhs ip %u bool %d  rhs ip %u bool %d\n",
   //          lhs.ip, lhs.isLocal, rhs.ip, rhs.isLocal);
      return lhs.ip < rhs.ip;
    }
  }
};
#endif

void CallWithCompareFun()
{
    PdpNetworkLocalKey x1(10, false);
    PdpNetworkLocalKey x2(16, true);
    PdpNetworkLocalKey x3(15, true);
    PdpNetworkLocalKey x4(10, true);

    PdpNetworkLocalKey searchKey;
    searchKey.ip = 10;
    searchKey.isLocal = true;
    map <PdpNetworkLocalKey, uint32_t, classcomp > Ip_Key_Int_Map;

    Ip_Key_Int_Map.insert(map <PdpNetworkLocalKey, uint32_t, classcomp>:: value_type(x1 , 1000));
    Ip_Key_Int_Map.insert(map <PdpNetworkLocalKey, uint32_t, classcomp>:: value_type(x2 , 2000));
    Ip_Key_Int_Map.insert(map <PdpNetworkLocalKey, uint32_t, classcomp>:: value_type(x3 , 3000));
    Ip_Key_Int_Map.insert(map <PdpNetworkLocalKey, uint32_t, classcomp>:: value_type(x4 , 4000));

    printf("IP_KEY_SIZE %d\n", Ip_Key_Int_Map.size());

#if 1
    map <PdpNetworkLocalKey, uint32_t, classcomp>::iterator it ;
    for(it = Ip_Key_Int_Map.begin(); it != Ip_Key_Int_Map.end(); it++)
    {
      printf("got %u  bool %d\n", (*it).first.ip, (*it).first.isLocal);

    }
#else

    map <PdpNetworkLocalKey, uint32_t, classcomp> :: const_iterator it = Ip_Key_Int_Map.upper_bound(x4);
    while(it != Ip_Key_Int_Map.end())
    {
      printf("got %u  bool %d\n", (*it).first.ip, (*it).first.isLocal);
      it++;
    }
#endif
    map <PdpNetworkLocalKey, uint32_t, classcomp>::iterator it1 ;
    it1 = Ip_Key_Int_Map.find(searchKey);
    if (it1 != Ip_Key_Int_Map.end())
    {
      printf("got %u  bool %d value %d\n", (*it1).first.ip, (*it1).first.isLocal, (*it1).second);
    }

}

#if 1
void CallWithoutCompareFun()
{
    PdpNetworkLocalKey x1(10, false);
    PdpNetworkLocalKey x2(16, true);
    PdpNetworkLocalKey x3(15, true);
    PdpNetworkLocalKey x4(10, true);

    PdpNetworkLocalKey searchKey;
    searchKey.ip = 10;
    searchKey.isLocal = true;

    if(searchKey == x3)
    {
      printf("search key is equal to one of the element\n");
    }
    else
    {
      printf("search key is not equal to any one \n");
    }
 
    map <PdpNetworkLocalKey, uint32_t > Ip_Key_Int_Map;

    Ip_Key_Int_Map.insert(map <PdpNetworkLocalKey, uint32_t >:: value_type(x1 , 1000));
    Ip_Key_Int_Map.insert(map <PdpNetworkLocalKey, uint32_t >:: value_type(x2 , 2000));
    Ip_Key_Int_Map.insert(map <PdpNetworkLocalKey, uint32_t >:: value_type(x3 , 3000));
    Ip_Key_Int_Map.insert(map <PdpNetworkLocalKey, uint32_t >:: value_type(x4 , 4000));
    printf("IP_KEY_SIZE %d\n", Ip_Key_Int_Map.size());

#if 1
    map <PdpNetworkLocalKey, uint32_t>::iterator it ;
    for(it = Ip_Key_Int_Map.begin(); it != Ip_Key_Int_Map.end(); it++)
    {
      printf("got %u  bool %d\n", (*it).first.ip, (*it).first.isLocal);

    }
#else

    map <PdpNetworkLocalKey, uint32_t> :: const_iterator it = Ip_Key_Int_Map.upper_bound(x4);
    while(it != Ip_Key_Int_Map.end())
    {
      printf("got %u  bool %d\n", (*it).first.ip, (*it).first.isLocal);
      it++;
    }
#endif

#if 0
    //erase element
    Ip_Key_Int_Map.erase(x4);
    printf("After erasing map entry \n \n");
    map <PdpNetworkLocalKey, uint32_t>::iterator it2 ;
    for(it2 = Ip_Key_Int_Map.begin(); it2 != Ip_Key_Int_Map.end(); it2++)
    {
      printf("got %u  bool %d\n", (*it2).first.ip, (*it2).first.isLocal);

    }
#endif

    map <PdpNetworkLocalKey, uint32_t>::iterator it1 ;
    it1 = Ip_Key_Int_Map.find(searchKey);
    if (it1 != Ip_Key_Int_Map.end())
    {
      printf("got %u  bool %d value %d\n", (*it1).first.ip, (*it1).first.isLocal, (*it1).second);
    }
}
#endif

int main ()
{
    //CallWithCompareFun();
    CallWithoutCompareFun();
    return 0;
}
