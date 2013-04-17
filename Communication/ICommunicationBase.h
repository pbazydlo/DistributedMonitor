#ifndef _ICommunicationBase_
#define _ICommunicationBase_

#include "Message.h"

#define MASTER	0
#define SLAVE 	1
#define MSG_TYPE_MUTEX_REQUEST	0
#define MSG_TYPE_MUTEX_ACCEPT	1

class ICommunicationBase {
public:
	virtual void Init(int masterOrSlave);
	virtual void Send(int receiver, int messageType);
	virtual Message* Receive();
	virtual void Broadcast(int messageType);
};

#endif
