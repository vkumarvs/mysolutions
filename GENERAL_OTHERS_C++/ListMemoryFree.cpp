#include <iostream>
#include <list>
using namespace std;
enum TraceType
{
    INCOMING_TRACE = 1,
    OUTGOING_TRACE,
    API_TRACE,
    SYSTEM_RESOURCE_TRACE,
    SYS_ERROR_TRACE,
    ALARM_ERROR_TRACE,
    INIT_TRACE,
    PROTO_EXCEPTION_TRACE,
    DEFAULT_NOT_USED_TRACE1,
    DEFAULT_NOT_USED_TRACE2,
    ALL_TRACE,
    GENERAL_ERROR_TRACE
};

struct LinkSlcMap
{
    std::string mLinkName;
    std::string mProfileName;
    uint8_t slc;
    bool linkInfo;
 
    ~LinkSlcMap()
    {
      printf("structure destructur called\n");
    }
};

class example
{
public:
    example(){}
    ~example();
    void add(LinkSlcMap * link);
private:
    list <LinkSlcMap *> m_Link;
    list <LinkSlcMap >  *d;
};

example::~example()
    {
      printf("Class Destructure called\n");
      LinkSlcMap *link;
      list <LinkSlcMap *>::iterator it;                                         
      for(it=m_Link.begin();it!=m_Link.end();it++)                              
      {
        link = *it;
        delete link;
      }
    }
void example::add(LinkSlcMap * link)
{
     m_Link.push_back(link);
}

int main()
{
    example *ex = new example;
    LinkSlcMap *link;
    for(int i=0; i<5; i++)
    {
      link = new LinkSlcMap;
      link->slc = i;
      ex->add(link);
    }
    delete ex;
    
    return 0;
}
