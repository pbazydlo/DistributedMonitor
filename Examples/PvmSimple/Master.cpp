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
 pvmCom->Broadcast(5);

 Message* msg = NULL;
 while((msg = pvmCom->Receive()) == NULL)
 {
  sleep(1);
 }
 cout<<"First message\n";
 Handle(msg);

 delete pvmCom;
}
