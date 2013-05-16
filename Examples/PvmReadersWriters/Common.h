#include "../../Communication/PvmCommunicationBase.h"
#include "../../DistributedMonitorBase.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define SLAVENUM 4

class ReadersWriters : public DistributedMonitor::DistributedMonitorBase
{
private:
	int readersCount = 0;
	int writersCount = 0;
	
public:
	ReadersWriters(ICommunicationBase* communicationBase) : DistributedMonitor::DistributedMonitorBase(communicationBase) {}

	void WantsToRead();
	void FinishReading();
	
	void WantsToWrite();
	void FinishWriting();	

protected:
	char* Serialize();
	void Deserialize(char* serializedContent);
};

