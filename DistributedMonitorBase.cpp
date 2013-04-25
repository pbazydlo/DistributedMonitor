#include "DistributedMonitorBase.h"
#include "Logging/Logger.h"
#include <stdio.h>
#include <unistd.h>
namespace DistributedMonitor{

#define DMB_MSG_ENTRY_REQUEST	100
#define DMB_MSG_ENTRY_ACCEPT	101
	
	int DistributedMonitorBase::NextMonitorId = 0;

	DistributedMonitorBase::DistributedMonitorBase(ICommunicationBase* communicationBase)
	{
		Logger* log = new Logger();
		log->Log("Initialising DistributedMonitorBase", LOG_DEBUG);
		this->_communicationBase = communicationBase;
		this->_locked = false;
		this->UpdateMonitorId();
		delete log;
	}

	void DistributedMonitorBase::UpdateMonitorId()
	{
		this->_monitorId = NextMonitorId;
		NextMonitorId=(NextMonitorId+1)%10;
	}

	void DistributedMonitorBase::Lock()
	{
		if(this->_locked)
		{
			return;
		}

		Logger* log = new Logger();
		this->_locked = true;

		int numberOfCoparticipants = this->_communicationBase->GetNumberOfSlaves();

		int numberOfAccepts=0;
		int myTid = this->_communicationBase->GetTid();

		this->_communicationBase->Broadcast(DMB_MSG_ENTRY_REQUEST, this->_monitorId);
		char numOfCo[100];
			sprintf(numOfCo, "%d / %d",numberOfAccepts,numberOfCoparticipants);
			log->Log(numOfCo, LOG_DEBUG);
		while(numberOfAccepts<numberOfCoparticipants)
		{
			sprintf(numOfCo, "%d / %d",numberOfAccepts,numberOfCoparticipants);
			log->Log(numOfCo, LOG_DEBUG);
			Message* msg = this->_communicationBase->Receive();
			if(msg != NULL)
			{
				switch(msg->MessageType)
				{
					case DMB_MSG_ENTRY_ACCEPT:
						log->Log("Got accept", LOG_DEBUG);
						numberOfAccepts++;
						break;
					case DMB_MSG_ENTRY_REQUEST:
						if((msg->MessagePriority < this->_monitorId) || 
						   (msg->Sender > myTid && msg->MessagePriority == this->_monitorId))
						{
							// let him in -> he has bigger priority
							this->_communicationBase->Send(msg->Sender, DMB_MSG_ENTRY_ACCEPT, this->_monitorId);
							log->Log("I'm weaker", LOG_DEBUG);
						}
						else
						{
							// i have bigger priority -> i'm going in just remember to notify him later
							this->_unlockPeers.push(msg->Sender);
							log->Log("I'm stronger", LOG_DEBUG);
						}

						break;
					default:
						// WHAT IF IT IS DATA
						break;
				};

				delete msg;
			}

		}

		delete log;
	}

	void DistributedMonitorBase::Unlock()
	{
		if(!this->_locked)
		{
			return;
		}
		
		Logger* log = new Logger();
		log->Log("Beginnig unlock", LOG_DEBUG);
		this->_locked = false;
		// free all waiting
		while(!this->_unlockPeers.empty())
		{
			int receiver = this->_unlockPeers.front();
			this->_unlockPeers.pop();
			this->_communicationBase->Send(receiver, DMB_MSG_ENTRY_ACCEPT, this->_monitorId);
			log->Log("Unlock", LOG_DEBUG);
			char str[100];
			sprintf(str, "%d", receiver);
			log->Log(str, LOG_DEBUG);
		}

		this->UpdateMonitorId();

		delete log;
	}

}
