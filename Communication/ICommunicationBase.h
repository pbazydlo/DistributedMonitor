#ifndef _ICommunicationBase_
#define _ICommunicationBase_

#include "Message.h"
#include <stddef.h>

#define MASTER	0
#define SLAVE 	1
#define BLOCKING false
#define NOTBLOCKING true

class ICommunicationBase {
public:
	ICommunicationBase();
	virtual void Init(int masterOrSlave) = 0;
	virtual void Send(int receiver, int messageType, int messagePriority) = 0;
	virtual Message* Receive(bool notBlocking = false) = 0;
	virtual void Broadcast(int messageType, int messagePriority) = 0;
	virtual void BroadcastData(int messageType, char* data, int messagePriority) = 0;
	virtual int GetNumberOfSlaves() = 0;
	virtual void SetDesiredNumberOfSlaves(int numberOfSlaves) = 0;
	virtual int GetTid() = 0;
	virtual void SetMessageHandlingFunction(void (*function)(Message*)) = 0;
};

#endif
