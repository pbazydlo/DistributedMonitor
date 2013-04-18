#include "Common.h"

using namespace std;

void Handle(Message* msg)
{
  cout<<"SENDER: "<<msg->Sender<<" TAG "<<msg->MessageType<<endl;
  delete msg;
}

int main()
{
 PvmCommunicationBase* pvmCom = PvmCommunicationBase::GetInstance();
 pvmCom->SetDesiredNumberOfSlaves(SLAVENUM);
 pvmCom->SetMessageHandlingFunction(Handle);
 pvmCom->Init(MASTER);
 pvmCom->Broadcast(5);
 sleep(5);
 delete pvmCom;
}
