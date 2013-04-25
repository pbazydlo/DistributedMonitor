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

 int tid = pvmCom->GetTid();
 mbase = new DistributedMonitor::DistributedMonitorBase(pvmCom);

 while(true)
 {
 	mbase->Lock();
	std::cout<<tid<<" is inside\n";
	sleep(1);
	std::cout<<tid<<" goes out\n";
	mbase->Unlock();
	sleep(1);
 }

 delete pvmCom;
}
