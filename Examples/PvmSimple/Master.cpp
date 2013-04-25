#include "Common.h"

using namespace std;

void Handle(Message* msg)
{
  cout<<"SENDER: "<<msg->Sender<<" TAG "<<msg->MessageType<<endl;
  delete msg;
}

int main()
{
 cout<<"start\n";
 PvmCommunicationBase* pvmCom = PvmCommunicationBase::GetInstance();
 pvmCom->SetDesiredNumberOfSlaves(SLAVENUM);
// pvmCom->SetMessageHandlingFunction(Handle);
 cout<<"init\n";
 pvmCom->Init(MASTER);
 int tid = pvmCom->GetTid();
 mbase = new DistributedMonitor::DistributedMonitorBase(pvmCom);

 while(true)
 {
 	mbase->Lock();
	cout<<tid<<" is inside\n";
	sleep(1);
	cout<<tid<<" goes out\n";
	mbase->Unlock();
//	sleep(1);
 }

 delete pvmCom;
}
