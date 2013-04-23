#include "Common.h"
#include <iostream>

PvmCommunicationBase* pvmCom;

void Handle(Message* msg)
{
  std::cout<<"GOT MESSAGE - ANSWER";
  pvmCom->Send(msg->Sender, 5);
  delete msg;
}

int main()
{
 pvmCom = PvmCommunicationBase::GetInstance();
 pvmCom->SetDesiredNumberOfSlaves(SLAVENUM);
 pvmCom->SetMessageHandlingFunction(Handle);
 pvmCom->Init(SLAVE);
// std::cout<<"Receive"<<std::endl;
// Message* msg = pvmCom->Receive();
// std::cout<<"Send to "<<msg->Sender<<std::endl;;
// pvmCom->Send(msg->Sender, 5);
// delete msg;
 sleep(3);
 delete pvmCom;
}
