#include "../../Communication/PvmCommunicationBase.h"
#include "../../DistributedMonitorBase.h"
#include "ProducerCondition.h"
#include "ConsumerCondition.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#ifndef COMMON
#define COMMON

#define SLAVENUM 1
#define CAPACITY 5
class Buffer : public DistributedMonitor::DistributedMonitorBase
{
private:
	int elements[CAPACITY] = {0};
	int putPosition = 0;
	int getPosition = 0;
	int counter = 0;
	
public:
	Buffer(ICommunicationBase* communicationBase) : DistributedMonitor::DistributedMonitorBase(communicationBase) {}

	void Put(int element);
	int Get();
	int GetCounter();

protected:
	char* Serialize();
	void Deserialize(char* serializedContent);
};
#endif
