#include <iostream>
using namespace std;

//Abstract State Class
class RanapPdpBaseState
{
public:
   RanapPdpBaseState(){}
   virtual ~RanapPdpBaseState(){} //Why virtual here?

   //copy constructor
   RanapPdpBaseState(const RanapPdpBaseState &rhs)
   {
     cout<<"calling copy constructor"<<endl;
   }

   //assignment operator
   RanapPdpBaseState & operator =(const RanapPdpBaseState &rhs)
   {
     if(this != &rhs) //Why we need this here? 
     {
       cout<<"Assignment operator is called" <<endl;
     }
     return *this;
   }

private:
};

//Concrete State classe A
class RanapPdpStateIdle:public RanapPdpBaseState
{
public:
    RanapPdpStateIdle(){}
    ~RanapPdpStateIdle(){}
 int x;
};

int main()
{   
    RanapPdpStateIdle x, y;
    RanapPdpBaseState org(x), org1(y);
    org = org1;
 // cout<<"value of x is"<<org <<endl;
    return 0;
}


