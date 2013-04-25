#include <pvm3.h>
#include "PvmCommunicationBase.h"
#include "../Logging/Logger.h"
#include <stdio.h>
#include <unistd.h>
#define SENDTAG 1

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


 if(masterOrSlave==MASTER)
 {
  log->Log("Init{MASTER}", LOG_DEBUG);
  this->_tids = new int[this->_desiredNumberOfSlaves];
  this->_nproc = pvm_spawn(SLAVENAME, NULL, PvmTaskDefault, "", 
  	this->_desiredNumberOfSlaves, this->_tids);
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

void PvmCommunicationBase::Send(int receiver, int messageType)
{
 if(messageType<=0)
 {
  // LOG ERROR
  return;
 }

 pvm_initsend(PvmDataDefault);
 pvm_pkint(&this->_myTid, 1, 1);
 pvm_pkint(&messageType, 1, 1);
 pvm_send(receiver, SENDTAG);
}

void PvmCommunicationBase::Broadcast(int messageType)
{
 Logger* log = new Logger();
 log->Log("Broadcast", LOG_DEBUG);
 delete log;
 pvm_initsend(PvmDataDefault);
 pvm_pkint(&this->_myTid, 1, 1);
 pvm_pkint(&messageType, 1, 1);
 pvm_bcast(GROUPNAME, SENDTAG);
}

Message* PvmCommunicationBase::Receive()
{
 int bufid, sender, messageSize, messageType, messageTid;
 if(pvm_probe(-1, -1)==0)
 {
	return NULL;
 }

 bufid = pvm_recv(-1, -1);
 pvm_upkint(&sender, 1, 1);
 pvm_upkint(&messageType, 1, 1);
// pvm_bufinfo(bufid, &messageSize, &messageType, &messageTid);
 Message* result = new Message();
 result->Sender = sender;
 result->MessageType = messageType;
 return result;
}

int PvmCommunicationBase::GetTid()
{
 return this->_myTid;
}

void PvmCommunicationBase::SetMessageHandlingFunction(void (*function)(Message*))
{
 PvmCommunicationBase::_function=function;
}



















