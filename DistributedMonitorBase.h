#ifndef _DISTRIBUTEDMONITORBASE_
#define _DISTRIBUTEDMONITORBASE_

#include "Communication/ICommunicationBase.h"
//#include "../../ICommunicationBase.h"

namespace DistributedMonitor {

class DistributedMonitorBase {
private:
	static int NextMonitorId;
	ICommunicationBase* _communicationBase;
	int _monitorId;
public:
	DistributedMonitorBase(ICommunicationBase*);
	void Lock();
	void Unlock();

};

}

#endif
