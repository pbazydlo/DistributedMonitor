#include "Common.h"

using namespace std;

int main()
{
 cout<<"start\n";
 PvmCommunicationBase* pvmCom = PvmCommunicationBase::GetInstance();
 pvmCom->SetDesiredNumberOfSlaves(SLAVENUM);
 cout<<"init\n";
 pvmCom->Init(MASTER);
 int tid = pvmCom->GetTid();
 ReadersWriters* rw = new ReadersWriters(pvmCom);
 int counter=0;

 while(true)
 {
	rw->WantsToWrite();
	sleep(1);
	rw->FinishWriting();
 }

 delete pvmCom;
}
