#include "../../Communication/PvmCommunicationBase.h"
#include "../../DistributedMonitorBase.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define SLAVENUM 1

class MyMonitor : public DistributedMonitor::DistributedMonitorBase
{
private:
	int counter = 0;
public:
	MyMonitor(ICommunicationBase* communicationBase) : DistributedMonitor::DistributedMonitorBase(communicationBase) {}

	void Entry()
	{
		this->Lock();
		this->counter++;
		std::cout<<"MM"<<" is inside\n";
		sleep(1);
		std::cout<</*this->_communicationBase->GetTid()*/"MM"<<" goes out\n";
		this->Unlock();
	}
protected:
	char* Serialize()
	{
		char* data = new char[100];
		sprintf(data,"%d",this->counter);
		return data;
	}

	void Deserialize(char* serializedContent)
	{
		std::cout<<"Deserializing... got: "<<serializedContent<<std::endl;
		sscanf(serializedContent,"%d",&(this->counter));
	}
};

MyMonitor* mbase;
