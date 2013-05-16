#include "Common.h"

using namespace std;

int main(int argc, char** argv)
{
 cout<<"start\n";
 MpiCommunicationBase* mpiCom = MpiCommunicationBase::GetInstance();
 cout<<"init\n";
 mpiCom->Init(argc, argv);
 int tid = mpiCom->GetTid();
 mbase = new MyMonitor(mpiCom);

 while(true)
 {
	mbase->Entry();
	sleep(1);
 }

 delete mpiCom;
}
