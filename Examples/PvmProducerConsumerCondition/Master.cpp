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
 Buffer* buffer = new Buffer(pvmCom);
 int counter=0;

 while(true)
 {
	buffer->Put(counter++);
	sleep(1);
 }

 delete pvmCom;
}
