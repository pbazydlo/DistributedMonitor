#include "Common.h"
#include <iostream>

PvmCommunicationBase* pvmCom;

int main()
{
 pvmCom = PvmCommunicationBase::GetInstance();
 pvmCom->SetDesiredNumberOfSlaves(SLAVENUM);
 pvmCom->Init(SLAVE);

 int tid = pvmCom->GetTid();
 ReadersWriters* rw = new ReadersWriters(pvmCom);

 while(true)
 {
	rw->WantsToRead();
	sleep(1);
	rw->FinishReading();
	sleep(4);
 }

 delete pvmCom;
}
