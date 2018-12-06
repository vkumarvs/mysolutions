#include <iostream>
#include<stdint.h>
#include <stdio.h>
using namespace std;

class example
{
public:
    example(){
      m_val = 0;
    }
    ~example()
    {
      cout<<"Destructure of class example called"<<endl;
    }
    void setVal(int x){m_val = x;}
    int getVal(){return m_val;}
private:
    int m_val;

};
int main()
{
    unsigned long x = 1959623432;
    //printf("size of unsigned long is (%d) value of x(%u)\n",sizeof(x),x);
  {
    example *ex = new example;
    ex->setVal(10);
    cout<<"value of x is "<<ex->getVal() <<endl;
    printf("vale is(%d)\n",ex->getVal());
  }

  {
    example ex ;
    ex.setVal(10);
    cout<<"2:value of x is "<<ex.getVal() <<endl;
    printf("2:vale is(%d)\n",ex.getVal());
  }

  {
    example *ex = new example; ;
    ex->setVal(10);
    cout<<"3:value of x is "<<ex->getVal() <<endl;
    printf("3:vale is(%d)\n",ex->getVal());
    delete ex;
  }


    return 0;
}
