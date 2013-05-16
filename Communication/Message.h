#ifndef _MESSAGE_
#define _MESSAGE_

struct Message {
	int Sender;
	int SenderClock;
	int MessagePriority;
	int MessageType;
	char* Data;
};

#endif
