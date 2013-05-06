#include "../../Communication/PvmCommunicationBase.h"
#include "../../DistributedMonitorBase.h"
#include <iostream>
#include <unistd.h>

#define SLAVENUM 1

class MyMonitor : public DistributedMonitor::DistributedMonitorBase
{
public:
	MyMonitor(ICommunicationBase* communicationBase) : DistributedMonitor::DistributedMonitorBase(communicationBase) {}
protected:
	char* Serialize()
	{
		return "";
	}

	void Deserialize(char* serializedContent)
	{
		// do sth
	}
};

MyMonitor* mbase;
