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
 pvmCom->SetMessageHandlingFunction(Handle);
 cout<<"init\n";
 pvmCom->Init(MASTER);
 pvmCom->Broadcast(5);
 sleep(3);
// while(1);
 delete pvmCom;
}
