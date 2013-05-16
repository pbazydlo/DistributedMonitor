#include "Common.h"
#include <iostream>

PvmCommunicationBase* pvmCom;

int main()
{
 pvmCom = PvmCommunicationBase::GetInstance();
 pvmCom->SetDesiredNumberOfSlaves(SLAVENUM);
// pvmCom->SetMessageHandlingFunction(Handle);
 pvmCom->Init(SLAVE);

 int tid = pvmCom->GetTid();
 mbase = new MyMonitor(pvmCom);

 while(true)
 {
	mbase->Entry();
	sleep(1);
 }

 delete pvmCom;
}
