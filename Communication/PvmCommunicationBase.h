#ifndef _PvmCommunicationBase_
#define _PvmCommunicationBase_

#include "ICommunicationBase.h"

#define SLAVENAME "Slave"
#define GROUPNAME "PVM_DIST_MON"

class PvmCommunicationBase : public ICommunicationBase {
private:
	PvmCommunicationBase();
	~PvmCommunicationBase();

	static PvmCommunicationBase* _instance;

	int _myTid;
	int _nproc;
	int _tids[SLAVENUM];
	int _desiredNumberOfSlaves;

public:

	static PvmCommunicationBase* GetInstance();
	void Init(int masterOrSlave);
	void Send(int receiver, int messageType);
	Message* Receive();
	void Broadast(int messageType);
	int GetNumberOfSlaves();
	void SetDesiredNumberOfSlaves(int numberOfSlaves);
};

#endif
