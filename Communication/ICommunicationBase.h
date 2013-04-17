#ifndef _ICommunicationBase_
#define _ICommunicationBase_

#include "Message.h"
#include <stddef.h>

#define MASTER	0
#define SLAVE 	1
#define MSG_TYPE_MUTEX_REQUEST	0
#define MSG_TYPE_MUTEX_ACCEPT	1

class ICommunicationBase {
public:
	ICommunicationBase();
	virtual void Init(int masterOrSlave) = 0;
	virtual void Send(int receiver, int messageType) = 0;
	virtual Message* Receive() = 0;
	virtual void Broadcast(int messageType) = 0;
};

#endif
