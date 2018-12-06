#include <iostream>
#include <state_machine.hpp>

/*Definition of static variables should be in source file*/
RanapPdpStateIdle        RanapPdpBaseState::m_pdpStateIdleState;
RanapPdpStateEstablished RanapPdpBaseState::m_pdpEstablishedState;
RanapPdpStateDeactivated RanapPdpBaseState::m_pdpDeactivatedState;

void RanapPdpSession::ChangeCurrentState(RanapPdpBaseState *newState)
{
    cout<<"Entering new State - Existing State(" <<m_currState->GetCurrentState()<<")"<<"---- >NewState("<<newState->GetCurrentState()<<")" <<endl;
    m_currState = newState;
}

RanapPdpSession:: RanapPdpSession()
{
    m_currState = &RanapPdpBaseState::m_pdpStateIdleState;
    cout<<"PdpSession state is  "<<m_currState->GetCurrentState() <<endl;
}

int main()
{
    RanapPdpSession *session = new RanapPdpSession();
    session->ChangeCurrentState(&RanapPdpBaseState::m_pdpStateIdleState);
    session->ChangeCurrentState(&RanapPdpBaseState::m_pdpEstablishedState);
    return 0;
}


