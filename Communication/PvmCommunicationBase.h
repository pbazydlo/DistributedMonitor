#ifndef _PvmCommunicationBase_
#define _PvmCommunicationBase_

#include "ICommunicationBase.h"

#define SLAVENAME "slave"
#define GROUPNAME "PVM_DIST_MON"

class PvmCommunicationBase : public ICommunicationBase {
private:
	PvmCommunicationBase();

	static PvmCommunicationBase* _instance;
	static int HandleMessage(int);
	static int _handleMessageMHID;
	static void (*_function)(Message*);

	int _myTid;
	int _nproc;
	int* _tids;
	int _desiredNumberOfSlaves;

public:
	static PvmCommunicationBase* GetInstance();

	~PvmCommunicationBase();
	void Init(int masterOrSlave);
	void Send(int receiver, int messageType, int messagePriority);
	Message* Receive();
	void Broadcast(int messageType, int messagePriority);
	void BroadcastData(int messageType, char* data, int messagePriority);
	int GetNumberOfSlaves();
	void SetDesiredNumberOfSlaves(int numberOfSlaves);
	int GetTid();
	void SetMessageHandlingFunction(void (*function)(Message*));
};

#endif
