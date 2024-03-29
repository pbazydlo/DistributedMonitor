#include <pvm3.h>
#include "PvmCommunicationBase.h"
#include "../Logging/Logger.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define SENDTAG 1
#define SENDDATATAG 2

// Create singleton
PvmCommunicationBase* PvmCommunicationBase::_instance = new PvmCommunicationBase();

void (*PvmCommunicationBase:: _function)(Message*) = 0;
int PvmCommunicationBase::_handleMessageMHID = 0;

int PvmCommunicationBase::HandleMessage(int messageId)
{
 Logger* log = new Logger();
 log->Log("Got message", LOG_DEBUG);
 int messageSize, messageTid;
 Message* msg = new Message();
 pvm_upkint(&msg->Sender, 1, 1);
 pvm_upkint(&msg->MessageType, 1, 1);
 delete log;
// pvm_bufinfo(messageId, &messageSize, &msg->MessageType, &messageTid);
 (*PvmCommunicationBase::_function)(msg);
 return 0;
}

// Returns singleton instance
PvmCommunicationBase* PvmCommunicationBase::GetInstance()
{
 return _instance;
}

PvmCommunicationBase::PvmCommunicationBase()
{
 Logger* log =new Logger();
 this->_myTid = pvm_mytid();
 log->Log("Initialised communication base", LOG_DEBUG);
 delete log;
}

PvmCommunicationBase::~PvmCommunicationBase()
{
 Logger* log =new Logger();
 log->Log("Cleaning up after communication base", LOG_DEBUG);
 delete log;

 delete[] this->_tids;
// pvm_delmhf(PvmCommunicationBase::_handleMessageMHID);
 pvm_exit();
}

void PvmCommunicationBase::Init(int masterOrSlave)
{
 if(this->_tids!=NULL)
 {
  return;
 }

 Logger* log = new Logger();
 log->Log("Init", LOG_DEBUG);
 this->_clock = 0;

 if(masterOrSlave==MASTER)
 {
  log->Log("Init{MASTER}", LOG_DEBUG);
  this->_tids = new int[this->_desiredNumberOfSlaves];
  this->_nproc = pvm_spawn(SLAVENAME, NULL, PvmTaskDefault, "", 
  	this->_desiredNumberOfSlaves, this->_tids);
 }
 else
 {
  this->_nproc=this->_desiredNumberOfSlaves;
 }

 log->Log("Init-Joingroup", LOG_DEBUG);
 pvm_joingroup(GROUPNAME);
 log->Log("Init-Approaching barrier", LOG_DEBUG);
 pvm_barrier(GROUPNAME, this->_desiredNumberOfSlaves+1);
/* IF UNCOMMENTED CORRECT DESTRUCTOR AS WELL!!
 if(masterOrSlave==MASTER)
 {
  int  ctx = pvm_newcontext();
  this->Broadcast(ctx);
  pvm_setcontext(ctx);
 }
 else
 {
  log->Log("Slave receive ctx", LOG_DEBUG);
  Message* msg = this->Receive();
  log->Log("Slave got ctx", LOG_DEBUG);
  pvm_setcontext(msg->MessageType);
  delete msg;
 }
	int ctx = pvm_getcontext();
	 PvmCommunicationBase::_handleMessageMHID = pvm_addmhf(-1,SENDTAG,ctx, HandleMessage);

	 log->Log("HANDLE MESSAGE MHID", LOG_DEBUG);
	 char mhid[200];
	 sprintf(mhid, "M: %d|C: %d", PvmCommunicationBase::_handleMessageMHID, ctx);
	 log->Log(mhid, LOG_DEBUG);
*/

 log->Log("End init", LOG_DEBUG);
 delete log;
}

int PvmCommunicationBase::GetNumberOfSlaves()
{
 return this->_nproc;
}

void PvmCommunicationBase::SetDesiredNumberOfSlaves(int numberOfSlaves)
{
 this->_desiredNumberOfSlaves=numberOfSlaves;
}

void PvmCommunicationBase::Send(int receiver, int messageType, int messagePriority)
{
 if(messageType<=0)
 {
  // LOG ERROR
  return;
 }

 this->_clock++;
 pvm_initsend(PvmDataDefault);
 pvm_pkint(&this->_myTid, 1, 1);
 pvm_pkint(&this->_clock, 1, 1);
 pvm_pkint(&messageType, 1, 1);
 pvm_pkint(&messagePriority, 1, 1);
 pvm_send(receiver, SENDTAG);
}

void PvmCommunicationBase::Broadcast(int messageType, int messagePriority)
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

void PvmCommunicationBase::BroadcastData(int messageType,char* data, int messagePriority)
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

Message* PvmCommunicationBase::Receive(bool notBlocking)
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

int PvmCommunicationBase::GetTid()
{
 return this->_myTid;
}

int PvmCommunicationBase::GetClock()
{
 return this->_clock;
}

void PvmCommunicationBase::SetMessageHandlingFunction(void (*function)(Message*))
{
 PvmCommunicationBase::_function=function;
}



















