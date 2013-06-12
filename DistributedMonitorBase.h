#ifndef _DISTRIBUTEDMONITORBASE_
#define _DISTRIBUTEDMONITORBASE_

#include "Communication/ICommunicationBase.h"
#include "ICondition.h"
#include <queue>
#include <pthread.h>

#define WaitWhile(a) while(a) { this->Unlock(); this->Lock(); }


namespace DistributedMonitor {

class DistributedMonitorBase {
private:
	static int NextMonitorId;
	int _monitorId;
	ICommunicationBase* _communicationBase;
	bool _locked;
	std::queue<int> _unlockPeers;
	void UpdateMonitorId();
	pthread_t _messageHandlingThread;
	static void* MessageHandlingThread(void*);
	void HandleMessages();
	bool _terminateMessageHandlingThread;
	void CreateMessageHandlingThread();
public:
	DistributedMonitorBase(ICommunicationBase*);

protected:
	virtual char* Serialize() = 0;
	virtual void Deserialize(char* serializedContent) = 0; 
	void Lock();
	void Unlock();
	ICondition *_currentCondition;
};

}

#endif
