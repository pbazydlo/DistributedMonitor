#include "Common.h"

using namespace std;

int main()
{
 PvmCommunicationBase* pvmCom = PvmCommunicationBase::GetInstance();
 pvmCom->SetDesiredNumberOfSlaves(SLAVENUM);
 pvmCom->Init(MASTER);
 pvmCom->Broadcast(5);
 for(int i=0;i<pvmCom->GetNumberOfSlaves();i++)
 {
  Message* msg = pvmCom->Receive();
  cout<<msg->Sender<<endl;
  delete msg;
 }
 delete pvmCom;
}
