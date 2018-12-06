#include <iostream>
#include <stdio.h>
#include <map>
#include <string.h>

using namespace std;

class Ctxt 
{
    int x;
public:
    Ctxt(){}
    ~Ctxt();
   int getInt(){return x;}
   void setInt(int p){x = p;}
};

class UseCtxt
{
public:
    UseCtxt(){}
    map<int , Ctxt*> m_CtxtMap;
    bool findCtxtObj(int id , Ctxt **ctxt);
    Ctxt* findCtxtObj(int id, Ctxt *ctxt);
    ~UseCtxt(){}
};

bool UseCtxt::findCtxtObj(int id, Ctxt **ctxt)
{
    map<int, Ctxt*>::iterator it;
    it = m_CtxtMap.find(id);
    if(it != m_CtxtMap.end())
    {
      *ctxt = it->second;
      printf("Pointer for obj is(%p)\n", *ctxt);
    }
    else
    {
      return false;
    }
    return true;
}

Ctxt* UseCtxt::findCtxtObj(int id, Ctxt *ctxt)
{
  map<int, Ctxt*>::iterator it;
  it = m_CtxtMap.find(id);
  if(it != m_CtxtMap.end())
  {
    ctxt = it->second;
    printf("without double pointer Pointer for obj is(%p)\n", ctxt);
  }
  return ctxt;
}

int main()
{
    Ctxt *test = NULL,*test1=NULL, *test2=NULL;
    Ctxt *val1 = new Ctxt();
    val1->setInt(1);
    
    UseCtxt use ;
    use.m_CtxtMap.insert(map <int, Ctxt*>:: value_type(1 ,val1));
    bool l = use.findCtxtObj(1,&test1);
    
    printf("Pointer for returned obj1 is(%p)  val(%d)\n", test1, test1->getInt());
    test1->setInt(10);
    
    l = use.findCtxtObj(1,&test1);
    printf("Pointer for returned obj1 is(%p)  val(%d)\n", test1, test1->getInt());
   
    return 0;
}
