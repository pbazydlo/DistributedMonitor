#include "DistributedMonitorBase.h"
#include "Logging/Logger.h"
#include <stdio.h>
#include <unistd.h>
namespace DistributedMonitor{

#define DMB_MSG_ENTRY_REQUEST		100
#define DMB_MSG_ENTRY_ACCEPT		101
#define DMB_MSG_SYNCHRONIZE		200
#define DMB_MSG_SYNCHRONIZE_ACCEPTED	201
	
	int DistributedMonitorBase::NextMonitorId = 0;

	DistributedMonitorBase::DistributedMonitorBase(ICommunicationBase* communicationBase)
	{
		Logger* log = new Logger();
		log->Log("Initialising DistributedMonitorBase", LOG_DEBUG);
		this->_communicationBase = communicationBase;
		this->_locked = false;
		this->UpdateMonitorId();
		this->CreateMessageHandlingThread();
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
		
		// terminate thread & join
		log->Log("Waiting for join...", LOG_DEBUG);
		this->_terminateMessageHandlingThread = true;
		pthread_join(this->_messageHandlingThread, NULL);		
		log->Log("Joined, proceeding with lock\n", LOG_DEBUG);
		this->_locked = true;

		int numberOfCoparticipants = this->_communicationBase->GetNumberOfSlaves();

		int numberOfAccepts=0;
		int myTid = this->_communicationBase->GetTid();

		this->_communicationBase->Broadcast(DMB_MSG_ENTRY_REQUEST, this->_monitorId);
		char numOfCo[100];
			sprintf(numOfCo, "[%d] %d / %d",myTid,numberOfAccepts,numberOfCoparticipants);
			log->Log(numOfCo, LOG_DEBUG);
		
		int myClock = this->_communicationBase->GetClock();

		while(numberOfAccepts<numberOfCoparticipants)
		{
			sprintf(numOfCo, "[%d] %d / %d",myTid,numberOfAccepts,numberOfCoparticipants);
			log->Log(numOfCo, LOG_DEBUG);
			Message* msg = this->_communicationBase->Receive(BLOCKING);
			if(msg != NULL)
			{
				switch(msg->MessageType)
				{
					case DMB_MSG_ENTRY_ACCEPT:
						sprintf(numOfCo, "Got accept from %x", msg->Sender);
						log->Log(numOfCo, LOG_DEBUG);
						numberOfAccepts++;
						break;
					case DMB_MSG_ENTRY_REQUEST:
						sprintf(numOfCo, "sndClk: %d, myClk: %d, sndTid>myTid: %d", 
								msg->SenderClock, 
								myClock,
								msg->Sender>myTid);
						log->Log(numOfCo, LOG_DEBUG);

						if((msg->SenderClock<myClock)
							 || (msg->SenderClock==myClock && msg->Sender > myTid ))
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
					case DMB_MSG_SYNCHRONIZE:
						log->Log("Got serialized data", LOG_DEBUG);
						this->Deserialize(msg->Data);
						delete [] msg->Data;
						log->Log("Deserialzed data, sending SYNC_ACCEPT", LOG_DEBUG);
						this->_communicationBase->Send(msg->Sender, DMB_MSG_SYNCHRONIZE_ACCEPTED, this->_monitorId);
						break;
					default:
						sprintf(numOfCo, "Unexpected message while waiting for lock - %d", msg->MessageType);
						log->Log(numOfCo, LOG_DEBUG);
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
		char *data = this->Serialize();
		log->Log("Sending serialized data", LOG_DEBUG);
		this->_communicationBase->BroadcastData(DMB_MSG_SYNCHRONIZE, data, this->_monitorId);
		delete[] data;
		// get accepts from all peers
		int numberOfCoparticipants = this->_communicationBase->GetNumberOfSlaves();
		int numberOfAccepts=0;
		while(numberOfAccepts<numberOfCoparticipants)
		{
			Message* msg = this->_communicationBase->Receive();
			if(msg != NULL)
			{
				switch(msg->MessageType)
				{
					case DMB_MSG_SYNCHRONIZE_ACCEPTED:
						numberOfAccepts++;
						break;
					case DMB_MSG_ENTRY_REQUEST:
						// we will send the ENTRY_ACCEPT message after sync is complete
						this->_unlockPeers.push(msg->Sender);
						break;
					default:
						char m[500];
						sprintf(m, "Unexpected message during unlock - %d", msg->MessageType);
						log->Log(m, LOG_DEBUG);
				};
			}
		}
		
		log->Log("Data has been synchronized... finalizing unlock", LOG_DEBUG);
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
		
		this->CreateMessageHandlingThread();

		delete log;

	}

void* DistributedMonitorBase::MessageHandlingThread(void* p) {
	reinterpret_cast<DistributedMonitorBase*>(p)->HandleMessages();
	pthread_exit(NULL);
	return NULL;
}

void DistributedMonitorBase::HandleMessages() {
	bool isConditionMet=false;
	while(!this->_terminateMessageHandlingThread && isConditionMet)
	{
		Message* msg = this->_communicationBase->Receive(NOTBLOCKING);
		if(msg != NULL)
		{
			switch(msg->MessageType)
			{
				case DMB_MSG_ENTRY_REQUEST:
					this->_communicationBase->Send(msg->Sender, DMB_MSG_ENTRY_ACCEPT, this->_monitorId);
					break;
				case DMB_MSG_SYNCHRONIZE:
					this->Deserialize(msg->Data);
					delete [] msg->Data;
					this->_communicationBase->Send(msg->Sender, DMB_MSG_SYNCHRONIZE_ACCEPTED, this->_monitorId);
					if(this->_currentCondition != NULL)
					{
						isConditionMet = this->_currentCondition->IsMet();
					}
					else
					{
						isConditionMet = true;
					}

					break;
				default:
					break;
			};

			delete msg;
		}

		sleep(1);
	}
}

void DistributedMonitorBase::CreateMessageHandlingThread() {
	// start message handling thread
	this->_terminateMessageHandlingThread = false;
	pthread_create(&(this->_messageHandlingThread), NULL, MessageHandlingThread, (void*)this);
}
	
}
