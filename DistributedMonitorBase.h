#ifndef _DISTRIBUTEDMONITORBASE_
#define _DISTRIBUTEDMONITORBASE_

#include "Communication/ICommunicationBase.h"
#include <queue>
#include <pthread.h>
//#include "../../ICommunicationBase.h"

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
};

}

#endif
