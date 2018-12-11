#include <iostream>
#include <map>
#include <string>
#include<list>
using namespace std;
struct LinkSlcMap
{
    string mLinkName;
    int slc;
};
class ConfigPath
{
public:
    ConfigPath()
    {
      m_name = "VIPIN DAHIYA";
    }
      ~ConfigPath(){}
  string getName(){return m_name;};
  string setName(string name){m_name = name;};
  void access();
  void clear(){m_name.clear();};
private:
   string m_name;
};
void ConfigPath::access()
{

}
int main()
{
   ConfigPath *m1 = new ConfigPath();
   ConfigPath *m2 = new ConfigPath();
   ConfigPath *m3 = new ConfigPath();
   LinkSlcMap *a1 = new LinkSlcMap;
   a1->mLinkName = "RAM";
   a1->slc = 10;
   cout<<"before string is "<<m3->getName() <<endl;
   m3->clear();
   if(!m3->getName().empty())
   cout <<"string is "<<m3->getName() <<endl;
   map <string, ConfigPath *> name_path;
   string s1 = "V1";
   string s2 = "V2qqq";
   string s3 = "V2";
   list<string> name;
   list<LinkSlcMap *> linkMap;
  linkMap.push_back(a1);
  list<LinkSlcMap *>::iterator k;
  for(k=linkMap.begin(); k!=linkMap.end(); k++)
  {
     cout << "structure link name is "<<(*k)->mLinkName <<endl;
     cout << "structure slc is "<<(*k)->slc <<endl;
  }
 //  if(s2 == s3)
    // cout <<"I am vipin "<<endl;
  name.push_back(s1);
  name.push_back(s2);
  name.push_back(s3);
  list<string>::iterator i;
  for(i=name.begin(); i!=name.end(); i++)
  {
    if((*i) == s2 )
    {
      break;
    }
  }
     name.erase(i);
 // i=name.begin();
 // advance(i,2);
  
  for(i=name.begin(); i!=name.end(); i++)
  {
     cout << (*i) <<endl;
  }
  name_path.insert(map <string, ConfigPath *>:: value_type(s1 ,m1));
  name_path.insert(map <string, ConfigPath *>:: value_type(s2 ,m2));
  map<string,ConfigPath *>::iterator it;
  for(it = name_path.begin(); it != name_path.end(); it++)
  {
    ConfigPath *p = (*it).second;
    p->getName();
  }
return 0;
}
