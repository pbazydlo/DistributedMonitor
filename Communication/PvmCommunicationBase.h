#ifndef _PvmCommunicationBase_
#define _PvmCommunicationBase_

#include "ICommunicationBase.h"

class PvmCommunicationBase : public ICommunicationBase {
public:
	void Send();
	void Receive();
	void Broadast();
};

#endif
