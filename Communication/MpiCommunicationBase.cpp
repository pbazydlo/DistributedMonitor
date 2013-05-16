#include <mpi.h>
#include "MpiCommunicationBase.h"
#include "../Logging/Logger.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define SENDTAG 1
#define SENDDATATAG 2

// Create singleton
MpiCommunicationBase* MpiCommunicationBase::_instance = new MpiCommunicationBase();

void (*MpiCommunicationBase:: _function)(Message*) = 0;
int MpiCommunicationBase::_handleMessageMHID = 0;

int MpiCommunicationBase::HandleMessage(int messageId)
{
 Logger* log = new Logger();
 log->Log("Got message", LOG_DEBUG);
 int messageSize, messageTid;
 Message* msg = new Message();
 pvm_upkint(&msg->Sender, 1, 1);
 pvm_upkint(&msg->MessageType, 1, 1);
 delete log;
// pvm_bufinfo(messageId, &messageSize, &msg->MessageType, &messageTid);
 (*MpiCommunicationBase::_function)(msg);
 return 0;
}

// Returns singleton instance
MpiCommunicationBase* MpiCommunicationBase::GetInstance()
{
 return _instance;
}

MpiCommunicationBase::MpiCommunicationBase()
{
 Logger* log =new Logger();
 log->Log("Initialised communication base", LOG_DEBUG);
 delete log;
}

MpiCommunicationBase::~MpiCommunicationBase()
{
 Logger* log =new Logger();
 log->Log("Cleaning up after communication base", LOG_DEBUG);
 delete log;
 
 MPI_Finalize();
}

void MpiCommunicationBase::Init(int masterOrSlave)
{
}

void MpiCommunicationBase::Init(int argc, char** argv) {
 int rank, size;
 
 Logger* log = new Logger();
 log->Log("Init", LOG_DEBUG);
 
 MPI_Init( &argc, &argv );
  
 MPI_Comm_rank( MPI_COMM_WORLD, &rank );
 MPI_Comm_size( MPI_COMM_WORLD, &size );
  
 this->_myTid = rank;
 this->_nproc = this->_desiredNumberOfSlaves = size-1;

 this->_clock = 0;

 log->Log("Init-Approaching barrier", LOG_DEBUG);
 MPI_Barrier( MPI_COMM_WORLD );

 log->Log("End init", LOG_DEBUG);
 delete log;
}

int MpiCommunicationBase::GetNumberOfSlaves()
{
 return this->_nproc;
}

void MpiCommunicationBase::SetDesiredNumberOfSlaves(int numberOfSlaves)
{
}

void MpiCommunicationBase::Send(int receiver, int messageType, int messagePriority)
{
 if(messageType<=0)
 {
  // LOG ERROR
  return;
 }

 this->_clock++;
 
 MPI_Send( msg, MSG_SIZE, MPI_INT, (rank+1)%size, MSG_HELLO, MPI_COMM_WORLD );

pvm_initsend(PvmDataDefault);
 pvm_pkint(&this->_myTid, 1, 1);
 pvm_pkint(&this->_clock, 1, 1);
 pvm_pkint(&messageType, 1, 1);
 pvm_pkint(&messagePriority, 1, 1);
 pvm_send(receiver, SENDTAG);
}

void MpiCommunicationBase::Broadcast(int messageType, int messagePriority)
{
 Logger* log = new Logger();
 log->Log("Broadcast", LOG_DEBUG);
 delete log;
 this->_clock++;
 pvm_initsend(PvmDataDefault);
 pvm_pkint(&this->_myTid, 1, 1);
 pvm_pkint(&this->_clock, 1, 1);
 pvm_pkint(&messageType, 1, 1);
 pvm_pkint(&messagePriority, 1, 1);
 pvm_bcast(GROUPNAME, SENDTAG);
}

void MpiCommunicationBase::BroadcastData(int messageType,char* data, int messagePriority)
{
 Logger* log = new Logger();
 log->Log("Broadcast data", LOG_DEBUG);
 log->Log(data, LOG_DEBUG);
 delete log;
 this->_clock++;
 pvm_initsend(PvmDataDefault);
 pvm_pkint(&this->_myTid, 1, 1);
 pvm_pkint(&this->_clock, 1, 1);
 pvm_pkint(&messageType, 1, 1);
 pvm_pkint(&messagePriority, 1, 1);
 int dataLength = strlen(data);
 pvm_pkint(&dataLength, 1, 1);
 pvm_pkstr(data);
 pvm_bcast(GROUPNAME, SENDDATATAG);
}

Message* MpiCommunicationBase::Receive(bool notBlocking)
{
 Logger* log = new Logger();
 int bufid, sender, messageSize, messageType, messageTid,
	messagePriority, messageTag, senderClock;
 if(notBlocking && pvm_probe(-1, -1)==0)
 {
	return NULL;
 }

 bufid = pvm_recv(-1, -1);
 pvm_upkint(&sender, 1, 1);
 pvm_upkint(&senderClock, 1, 1);
 pvm_upkint(&messageType, 1, 1);
 pvm_upkint(&messagePriority, 1, 1);
 pvm_bufinfo(bufid, &messageSize, &messageTag, &messageTid);
 Message* result = new Message();
 result->Sender = sender;
 result->SenderClock = senderClock;
 result->MessageType = messageType;
 result->MessagePriority = messagePriority;
 if(messageTag == SENDDATATAG)
 {
	int dataLength=0;
	pvm_upkint(&dataLength, 1, 1);
	log->Log("Got data message", LOG_DEBUG);
	result->Data = new char[dataLength+1];
	result->Data[dataLength]= 0;
 	pvm_upkstr(result->Data);
	log->Log("Unpacked data from message", LOG_DEBUG);
 }
 
 if(this->_clock<senderClock)
 {
  this->_clock = senderClock;
 }

 delete log;
 return result;
}

int MpiCommunicationBase::GetTid()
{
 return this->_myTid;
}

int MpiCommunicationBase::GetClock()
{
 return this->_clock;
}

void MpiCommunicationBase::SetMessageHandlingFunction(void (*function)(Message*))
{
 MpiCommunicationBase::_function=function;
}



















