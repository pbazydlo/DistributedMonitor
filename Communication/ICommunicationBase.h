#ifndef _ICommunicationBase_
#define _ICommunicationBase_

class ICommunicationBase {
public:
	virtual void Send();
	virtual void Receive();
	virtual void Broadcast();
};

#endif
