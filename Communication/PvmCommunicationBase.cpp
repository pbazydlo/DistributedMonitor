#include <pvm3.h>
#include "PvmCommunicationBase.h"
#define SENDTAG 1

// Create singleton
PvmCommunicationBase* PvmCommunicationBase::_instance = new PvmCommunicationBase();

void (*PvmCommunicationBase:: _function)(Message*) = 0;
int PvmCommunicationBase::_handleMessageMHID = 0;

int PvmCommunicationBase::HandleMessage(int messageId)
{
 int messageSize, messageTid;
 Message* msg = new Message();
 pvm_upkint(&msg->Sender, 1, 1);
 pvm_upkint(&msg->MessageType, 1, 1);
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
 this->_myTid = pvm_mytid();
}

PvmCommunicationBase::~PvmCommunicationBase()
{
 delete[] this->_tids;
 pvm_delmhf(PvmCommunicationBase::_handleMessageMHID);
 pvm_exit();
}

void PvmCommunicationBase::Init(int masterOrSlave)
{
 if(this->_tids!=NULL)
 {
  return;
 }
 
 PvmCommunicationBase::_handleMessageMHID = pvm_addmhf(-1,1,-1, HandleMessage);
 if(masterOrSlave==MASTER)
 {
  this->_tids = new int[this->_desiredNumberOfSlaves];
  this->_nproc = pvm_spawn(SLAVENAME, NULL, PvmTaskDefault, "", 
  	this->_desiredNumberOfSlaves, this->_tids);
 }

 pvm_joingroup(GROUPNAME);
 pvm_barrier(GROUPNAME, this->_desiredNumberOfSlaves+1);
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
 pvm_initsend(PvmDataDefault);
 pvm_pkint(&this->_myTid, 1, 1);
 pvm_pkint(&messageType, 1, 1);
 pvm_bcast(GROUPNAME, SENDTAG);
}

Message* PvmCommunicationBase::Receive()
{
 int bufid, sender, messageSize, messageType, messageTid;
 bufid = pvm_recv(-1, -1);
 pvm_upkint(&sender, 1, 1);
 pvm_bufinfo(bufid, &messageSize, &messageType, &messageTid);
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



















