#include <iostream>
#include <map>
#include <string>
#include<stdint.h>
using namespace std;

class IuSession
{
public:
    IuSession()
       {
         x= 10;
          y=20;
       }
    int x;
    int y;

};
int main()
{
    bool x =true;
   IuSession *id = new IuSession;
   IuSession *id1 = new IuSession;
   map <uint64_t , IuSession*> connMap;
    pair <map<uint64_t, IuSession *>::iterator,bool> ret1;
   connMap.insert(map <uint64_t, IuSession*>:: value_type(10 , id));
   ret1 = connMap.insert(map <uint64_t, IuSession*>:: value_type(10 , id1));
   if(ret1.second == false)
   {
     printf("can not add same value with same key bool(%d)\n",x);
   }
   //connMap.erase(10);
   delete id;
   map<uint64_t, IuSession*>::iterator it;
   it = connMap.find(135204);
   if (it != connMap.end())
   {
     ((*it).second)->x = 30;
     printf("let me see %d %d\n",((*it).second)->x, ((*it).second)->y);
   }
   else
   {
     printf("let me not see\n");
   }
   return 0;
}

