#include "Common.h"

int main()
{
 PvmCommunicationBase* pvmCom = PvmCommunicationBase::GetInstance();
 pvmCom->SetDesiredNumberOfSlaves(SLAVENUM);
 pvmCom->Init(SLAVE);
 Message* msg = pvmCom->Receive();
 pvmCom->Send(msg->Sender, 5);
 delete msg;
 delete pvmCom;
}
