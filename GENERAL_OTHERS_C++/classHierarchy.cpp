#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>

using namespace std;
#if 0

1)For dynamic cast base class should be polymorphic

#endif
class TopLevel
{
public:
    TopLevel()
    {
      if(s_instance)
      {
        printf("attemp to create multiple instance of singleton\n");
        exit(1);
      }

      s_instance = this;
      z =10;
    }
    static TopLevel* GetInstance();
   virtual ~TopLevel(){};
   //~TopLevel(){};
   //virtual ~TopLevel(){};


    int GetZ(){return z;}
private:
    int z;
    static TopLevel *s_instance;
};

TopLevel *TopLevel::s_instance = NULL;


TopLevel* TopLevel::GetInstance()
{
      return s_instance;
}

class LowLevel:public TopLevel
{
public:
    LowLevel():TopLevel()
    {
      x = 20;
      y = 30;
    }

    ~LowLevel(){}
    int GetX(){return x;}

private:
    int x;
    int y;
};



int main()
{
    LowLevel *low = new LowLevel();
   // TopLevel *top = new TopLevel();      ---- wrong
    LowLevel *low1 = NULL;
    low1 = dynamic_cast<LowLevel*>(TopLevel::GetInstance());
   // low1 = (LowLevel*)(TopLevel::GetInstance()); ------ wrong way 
    printf("%u \n", low1->GetX());
    return 0;
}


