#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#endif

using namespace std;
class RanapPdpStateIdle;
class RanapPdpStateEstablished;
class RanapPdpStateDeactivated;

//Abstract State Class 
class RanapPdpBaseState
{
public:
   //Declration of static variables
   static RanapPdpStateIdle           m_pdpStateIdleState;
   static RanapPdpStateEstablished    m_pdpEstablishedState;
   static RanapPdpStateDeactivated    m_pdpDeactivatedState;
   RanapPdpBaseState(){}
   ~RanapPdpBaseState(){}

   virtual const char *GetCurrentState(){return "RanapPdpStateBaseState";}
private:
};

//Concrete State classe A
class RanapPdpStateIdle:public RanapPdpBaseState
{
public:
    const char *GetCurrentState(){return "RanapPdpStateIdleState";}
};

//Concrete State classe B
class RanapPdpStateEstablished:public RanapPdpBaseState
{
public:
    const char *GetCurrentState(){return "RanapPdpStateEstablishedState";}
};

//Concrete State classe C
class RanapPdpStateDeactivated:public RanapPdpBaseState
{
public:
    const char *GetCurrentState(){return "PdpDeactivatedState";}
};

//Context Class for which state machine has been implemented 
class RanapPdpSession
{
public:
  RanapPdpSession();
  ~RanapPdpSession(){}
  void ChangeCurrentState(RanapPdpBaseState *newState);

private:
  RanapPdpBaseState *m_currState;
};


