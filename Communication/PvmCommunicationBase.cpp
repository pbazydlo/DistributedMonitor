#include <pvm3.h>
#include "PvmCommunicationBase.h"


// Create singleton
PvmCommunicationBase* PvmCommunicationBase::_instance = new PvmCommunicationBase();

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
 pvm_exit();
}

void PvmCommunicationBase::Init(int masterOrSlave)
{
 if(masterOrSlave==MASTER)
 {
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
 if(this->_tids!=NULL)
 {
  delete[] this->_tids;
 }

 this->_tids = new int[this->_desiredNumberOfSlaves];
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
 pvm_send(receiver, messageType);
}

void PvmCommunicationBase::Broadcast(int messageType)
{
 pvm_initsend(PvmDataDefault);
 pvm_pkint(&this->_myTid, 1, 1);
 pvm_bcast(GROUPNAME, messageType);
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























