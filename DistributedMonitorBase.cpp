#include "DistributedMonitorBase.h"
namespace DistributedMonitor{

#define DMB_MSG_ENTRY_REQUEST	100
#define DMB_MSG_ENTRY_ACCEPT	101

DistributedMonitorBase::DistributedMonitorBase(ICommunicationBase* communicationBase)
{
 this->_communicationBase = communicationBase;
 // this->_monitorId = NextMonitorId;
 // NextMonitorId++;
}

void DistributedMonitorBase::Lock()
{/*
 int numberOfCoparticipants = this->_communicationBase->GetNumberOfSlaves();
 int numberOfAccepts=0;
 int myTid = this->_communicationBase->GetTid();

 this->_communicationBase.Broadcast(DMB_MSG_ENTRY_REQUEST);
 while(numberOfAccepts<numberOfCoparticipants)
 {
  Message* msg = this->_communicationBase.Receive();
  switch(msg->MessageType)
  {
  	case DMB_MSG_ENTRY_ACCEPT:
		numberOfAccepts++;
		break;
	case DMB_MSG_ENTRY_REQUEST:
		if(msg->Sender>myTid)
		{
		}
		break;
	default:
		// WHAT IF IT IS DATA
  };

  delete msg;
 }*/
}

}
