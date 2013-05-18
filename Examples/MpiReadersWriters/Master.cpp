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
 ReadersWriters* readersWriters = new ReadersWriters(mpiCom);
 readersWriters->SetMyId(tid);
 int counter=0;
 int isReader = tid % 2;
 int isWriter = 1 - isReader;
 if(isWriter) {
 	cout<<"["<<tid<<"] is writer\n";
 }
 else {
 	cout<<"["<<tid<<"] is reader\n";
 }
 
 while(true)
 {
	if(isWriter) {
	 readersWriters->WantsToWrite();
	 cout << "[" << tid << "] is writing for the " << (counter+1) << " time.\n";
	 sleep(1);
	 cout << "[" << tid << "] finished writing\n";
	 readersWriters->FinishWriting();
	} else {
	 readersWriters->WantsToRead();
	 cout << "[" << tid << "] is reading for the " << (counter+1) << " time.\n";
	 sleep(1);
	 cout << "[" << tid << "] finished reading\n";
	 readersWriters->FinishReading();
	 sleep(4);
	}
	counter++;
 }

 delete mpiCom;
}
