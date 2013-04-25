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
// pvmCom->SetMessageHandlingFunction(Handle);
 pvmCom->Init(SLAVE);

 Message* msg = NULL;
 while((msg = pvmCom->Receive()) == NULL)
 {
  sleep(1);
 }
 std::cout<<"Slave got message\n";
 Handle(msg);
// std::cout<<"Receive"<<std::endl;
// Message* msg = pvmCom->Receive();
// std::cout<<"Send to "<<msg->Sender<<std::endl;;
// pvmCom->Send(msg->Sender, 5);
// delete msg;

 delete pvmCom;
}
