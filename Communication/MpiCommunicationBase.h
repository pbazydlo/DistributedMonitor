#ifndef _MpiCommunicationBase_
#define _MpiCommunicationBase_

#include "ICommunicationBase.h"
#include <mpi.h>

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
	
	void RegisterSyncMsgType();
	void RegisterDataMsgType();
        MPI_Datatype _mpiSyncMsgType;
	MPI_Datatype _mpiDataMsgType;	
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
};

#define MPI_SYNC_MSG_ITEMS_COUNT 4
struct MpiSynchronizationMessage {
	int MyTid;
	int Clock;
	int MessageType;
	int MessagePriority;
};

#define MPI_DATA_MSG_ITEMS_COUNT 6
#define MPI_DATA_MSG_PAYLOAD_SIZE 256
struct MpiDataMessage {
	int MyTid;
	int Clock;
	int MessageType;
	int MessagePriority;
	int DataLength;
	char Data[MPI_DATA_MSG_PAYLOAD_SIZE];
};

#endif
