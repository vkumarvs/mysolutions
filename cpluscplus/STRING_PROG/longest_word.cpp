#include <iostream>
#include <list>
#include <string>
#include <map>
#include <array>
using namespace std;

void getLongestWord(array<string, 4> &list)
{
    printf("size of list is %zu \n", list.size());
    map <string, bool> hashMap ;
    map <string, bool>::iterator it1;
    map <string, bool>::iterator it;
    for(int i = 0; i < list.size(); i++)
    {
      hashMap.insert(map <string, bool>:: value_type(list.at(i) , true));
      printf("putting word into map %s \n", list.at(i).c_str());
    }

    for(int i = 0; i < list.size(); i++)
    {
      for(int y = 1; y <= list.at(i).length(); y++)
      {
        string left = list.at(i).substr(0,y);
        string right = list.at(i).substr(y);

        it1 = hashMap.find(left);
        it = hashMap.find(right);
        if ((it1 != hashMap.end() && (*it1).second) && (it != hashMap.end() && (*it).second))
        {
          printf("longest string is  %s\n", list.at(i).c_str());
        }
      }
    }
}

int main ()
{
  array <string, 4> _list;
  _list.at(0) = "hello";
  _list.at(1) = "wound";
  _list.at(2) = "hellowound";
  _list.at(3) = "threeone";

   getLongestWord(_list);
  return 0;
}

