#include "Common.h"

PvmCommunicationBase* pvmCom;

void Handle(Message* msg)
{
  pvmCom->Send(msg->Sender, 5);
  delete msg;
}

int main()
{
 pvmCom = PvmCommunicationBase::GetInstance();
 pvmCom->SetDesiredNumberOfSlaves(SLAVENUM);
 pvmCom->SetMessageHandlingFunction(Handle);
 pvmCom->Init(SLAVE);
 sleep(5);
 delete pvmCom;
}
