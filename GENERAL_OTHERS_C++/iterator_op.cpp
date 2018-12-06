#include <iostream>
#include <map>
#include <string>
#include<list>
#include <stdio.h>
#include <set>
using namespace std;


std::string& getGrpString(const char* separator, std::string& grpNames)
{
    printf("seprator %s , string is %s \n", separator, grpNames.c_str());
    std::set<std::string> m_group_set;
    std::set<std::string>::iterator it;

    /*insert some values*/
    m_group_set.insert("my name");
    m_group_set.insert();

    int count = 0;
    for (it=m_group_set.begin(); it != m_group_set.end(); it++)
    {
      if ((++count) != 1)
      {
        grpNames.append(separator);
      }

      grpNames.append(*it);

      //it++;
    }
    return grpNames;
}


int main()
{
    std::string myname;
    printf("return is %s \n", (getGrpString("?", myname)).c_str()); 
    return 0;
}
