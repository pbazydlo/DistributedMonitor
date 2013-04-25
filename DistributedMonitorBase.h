#ifndef _DISTRIBUTEDMONITORBASE_
#define _DISTRIBUTEDMONITORBASE_

#include "Communication/ICommunicationBase.h"
#include <queue>
//#include "../../ICommunicationBase.h"

namespace DistributedMonitor {

class DistributedMonitorBase {
private:
	static int NextMonitorId;
	ICommunicationBase* _communicationBase;
	int _monitorId;
	bool _locked;
	std::queue<int> _unlockPeers;
	void UpdateMonitorId();
public:
	DistributedMonitorBase(ICommunicationBase*);
	void Lock();
	void Unlock();

};

}

#endif
