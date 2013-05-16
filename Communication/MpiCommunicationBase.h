#ifndef _MpiCommunicationBase_
#define _MpiCommunicationBase_

#include "ICommunicationBase.h"

class MpiCommunicationBase : public ICommunicationBase {
private:
	MpiCommunicationBase();

	static MpiCommunicationBase* _instance;
	static int HandleMessage(int);
	static int _handleMessageMHID;
	static void (*_function)(Message*);

	int _myTid;
	int _nproc;
	int _desiredNumberOfSlaves;
	int _clock;
public:
	static MpiCommunicationBase* GetInstance();

	~MpiCommunicationBase();
	void Init(int masterOrSlave);
	void Init(int argc, char** argv);
	void Send(int receiver, int messageType, int messagePriority);
	Message* Receive(bool notBlocking = false);
	void Broadcast(int messageType, int messagePriority);
	void BroadcastData(int messageType, char* data, int messagePriority);
	int GetNumberOfSlaves();
	void SetDesiredNumberOfSlaves(int numberOfSlaves);
	int GetTid();
	int GetClock();
	void SetMessageHandlingFunction(void (*function)(Message*));
};

#endif
