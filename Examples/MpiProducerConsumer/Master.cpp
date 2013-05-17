#include "Common.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv)
{
 
 cout<<"start\n";
 MpiCommunicationBase* mpiCom = MpiCommunicationBase::GetInstance();
 cout<<"init\n";
 mpiCom->Init(argc, argv);
 int tid = mpiCom->GetTid();
 srand(time(NULL)+tid);
 Buffer* buffer = new Buffer(mpiCom);
 int counter=0;
 int isProducer = rand() & 1;
 int isConsumer = 1 - isProducer;
 
 while(true)
 {
	if(isProducer) {
	 cout << "[" << tid << "] item " << (counter+1) << " produced";
	 buffer->Put(counter++);
	} else {
	 cout << "[" << tid << "] item " << buffer->Get() << " consumed";
	}
	
	sleep(1);
 }

 delete mpiCom;
}
