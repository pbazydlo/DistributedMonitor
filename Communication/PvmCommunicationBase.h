#ifndef _PvmCommunicationBase_
#define _PvmCommunicationBase_

#include "ICommunicationBase.h"

#define SLAVENAME "Slave"
#define GROUPNAME "PVM_DIST_MON"

class PvmCommunicationBase : public ICommunicationBase {
private:
	PvmCommunicationBase();

	static PvmCommunicationBase* _instance;

	int _myTid;
	int _nproc;
	int* _tids;
	int _desiredNumberOfSlaves;

public:
	static PvmCommunicationBase* GetInstance();

	~PvmCommunicationBase();
	void Init(int masterOrSlave);
	void Send(int receiver, int messageType);
	Message* Receive();
	void Broadcast(int messageType);
	int GetNumberOfSlaves();
	void SetDesiredNumberOfSlaves(int numberOfSlaves);
};

#endif
