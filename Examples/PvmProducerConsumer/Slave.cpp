#include "Common.h"
#include <iostream>

PvmCommunicationBase* pvmCom;

int main()
{
 pvmCom = PvmCommunicationBase::GetInstance();
 pvmCom->SetDesiredNumberOfSlaves(SLAVENUM);
 pvmCom->Init(SLAVE);

 int tid = pvmCom->GetTid();
 Buffer* buffer = new Buffer(pvmCom);

 while(true)
 {
	buffer->Get();
	sleep(2);
 }

 delete pvmCom;
}
