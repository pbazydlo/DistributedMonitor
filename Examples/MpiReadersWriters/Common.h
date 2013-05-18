#include "../../Communication/MpiCommunicationBase.h"
#include "../../DistributedMonitorBase.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

class ReadersWriters : public DistributedMonitor::DistributedMonitorBase
{
private:
	int readersCount = 0;
	int writersCount = 0;
	int myId = 0;
	
public:
	ReadersWriters(ICommunicationBase* communicationBase) : DistributedMonitor::DistributedMonitorBase(communicationBase) {}

	void WantsToRead();
	void FinishReading();
	
	void WantsToWrite();
	void FinishWriting();	

	void SetMyId(int id);
protected:
	char* Serialize();
	void Deserialize(char* serializedContent);
};

