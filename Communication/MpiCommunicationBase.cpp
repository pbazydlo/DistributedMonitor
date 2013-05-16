#include <mpi.h>
#include "MpiCommunicationBase.h"
#include "../Logging/Logger.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h> //offsetof
#define SENDTAG 1
#define SENDDATATAG 2

// Create singleton
MpiCommunicationBase* MpiCommunicationBase::_instance = new MpiCommunicationBase();

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

 this->RegisterSyncMsgType();
 this->RegisterDataMsgType();
 
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
 

 MpiSynchronizationMessage mpiSyncMsg; //create instance of structure
 mpiSyncMsg.MyTid = this->_myTid;
 mpiSyncMsg.Clock = this->_clock;
 mpiSyncMsg.MessageType = messageType;
 mpiSyncMsg.MessagePriority = messagePriority; 
 MPI_Send( &mpiSyncMsg, 1, this->_mpiSyncMsgType, receiver, SENDTAG, MPI_COMM_WORLD );
}

void MpiCommunicationBase::Broadcast(int messageType, int messagePriority)
{
 Logger* log = new Logger();
 log->Log("Broadcast", LOG_DEBUG);
 delete log;
 this->_clock++;
 MpiSynchronizationMessage mpiSyncMsg; //create instance of structure
 mpiSyncMsg.MyTid = this->_myTid;
 mpiSyncMsg.Clock = this->_clock;
 mpiSyncMsg.MessageType = messageType;
 mpiSyncMsg.MessagePriority = messagePriority; 
 for(int i=0; i<this->_nproc+1; i++) {
  if(i!=this->_myTid) {
   MPI_Send( &mpiSyncMsg, 1, this->_mpiSyncMsgType, i, SENDTAG, MPI_COMM_WORLD );
  }
 }
}

void MpiCommunicationBase::BroadcastData(int messageType,char* data, int messagePriority)
{
 Logger* log = new Logger();
 log->Log("Broadcast data", LOG_DEBUG);
 log->Log(data, LOG_DEBUG);
 delete log;
 this->_clock++;
 MpiDataMessage mpiDataMsg; //create instance of structure
 mpiDataMsg.MyTid = this->_myTid;
 mpiDataMsg.Clock = this->_clock;
 mpiDataMsg.MessageType = messageType;
 mpiDataMsg.MessagePriority = messagePriority; 
 mpiDataMsg.DataLength = strlen(data);
 if(mpiDataMsg.DataLength>MPI_DATA_MSG_PAYLOAD_SIZE-1) {
  mpiDataMsg.DataLength = MPI_DATA_MSG_PAYLOAD_SIZE;
 }
 strncpy( mpiDataMsg.Data, data, mpiDataMsg.DataLength);
 for(int i=0; i<this->_nproc+1; i++) {
  if(i!=this->_myTid) {
   MPI_Send( &mpiDataMsg, 1, this->_mpiDataMsgType, i, SENDDATATAG, MPI_COMM_WORLD );
  }
 }
}

Message* MpiCommunicationBase::Receive(bool notBlocking)
{
 Logger* log = new Logger();
 int tag, sender, flag;
 MPI_Status status;
 
 // non-blocking probe
 MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
 
 // if there is no message and we don't want to wait...
 if(notBlocking && flag==false) {
 	return NULL;
 } 
  
 Message* result = new Message();
 
 // otherwise wait for the message
 if(MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status)==MPI_SUCCESS) {
   tag = status.MPI_TAG; //MPI_Get_tag(status, &tag);
   sender = status.MPI_SOURCE; //MPI_Get_source(status, &sender);
   if(tag == SENDTAG) {
	log->Log("Got synchronization message", LOG_DEBUG);
	MpiSynchronizationMessage mpiSyncMsg;
        MPI_Recv(&mpiSyncMsg, 1, this->_mpiSyncMsgType, sender, tag, MPI_COMM_WORLD, &status);
	result->Sender = mpiSyncMsg.MyTid;
	result->SenderClock = mpiSyncMsg.Clock;
	result->MessageType = mpiSyncMsg.MessageType;
	result->MessagePriority = mpiSyncMsg.MessagePriority;
   } else if(tag == SENDDATATAG) {
	log->Log("Got data message", LOG_DEBUG);
	MpiDataMessage mpiDataMsg;
        MPI_Recv(&mpiDataMsg, 1, this->_mpiDataMsgType, sender, tag, MPI_COMM_WORLD, &status);
	result->Sender = mpiDataMsg.MyTid;
	result->SenderClock = mpiDataMsg.Clock;
	result->MessageType = mpiDataMsg.MessageType;
	result->MessagePriority = mpiDataMsg.MessagePriority;
	std::cout << "Length of the message is " << mpiDataMsg.DataLength << "\n";
	result->Data = new char[mpiDataMsg.DataLength+1];
	strncpy(result->Data, mpiDataMsg.Data, mpiDataMsg.DataLength); 
	//std::cout << "Content of the message is " << result->Data << "\n\n";	
	log->Log("Unpacked data from message", LOG_DEBUG);
	result->Data[mpiDataMsg.DataLength]=0;
   }
 }

 if(this->_clock<result->SenderClock)
 {
  this->_clock = result->SenderClock;
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

void MpiCommunicationBase::RegisterSyncMsgType() 
{
 int blocklengths[MPI_SYNC_MSG_ITEMS_COUNT] = {1,1,1,1};
 MPI_Datatype types[MPI_SYNC_MSG_ITEMS_COUNT] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};
 MPI_Aint     offsets[MPI_SYNC_MSG_ITEMS_COUNT];

 offsets[0] = offsetof(struct MpiSynchronizationMessage, MyTid);
 offsets[1] = offsetof(struct MpiSynchronizationMessage, Clock);
 offsets[2] = offsetof(struct MpiSynchronizationMessage, MessageType);
 offsets[3] = offsetof(struct MpiSynchronizationMessage, MessagePriority);

 MPI_Type_create_struct(MPI_SYNC_MSG_ITEMS_COUNT, blocklengths, offsets, types, &(this->_mpiSyncMsgType));
 MPI_Type_commit(&(this->_mpiSyncMsgType));
}

void MpiCommunicationBase::RegisterDataMsgType() 
{
 int blocklengths[MPI_DATA_MSG_ITEMS_COUNT] = {1,1,1,1,1,MPI_DATA_MSG_PAYLOAD_SIZE};
 MPI_Datatype types[MPI_DATA_MSG_ITEMS_COUNT] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_CHAR};
 MPI_Aint     offsets[MPI_DATA_MSG_ITEMS_COUNT];

 offsets[0] = offsetof(struct MpiDataMessage, MyTid);
 offsets[1] = offsetof(struct MpiDataMessage, Clock);
 offsets[2] = offsetof(struct MpiDataMessage, MessageType);
 offsets[3] = offsetof(struct MpiDataMessage, MessagePriority);
 offsets[4] = offsetof(struct MpiDataMessage, DataLength);
 offsets[5] = offsetof(struct MpiDataMessage, Data);

 MPI_Type_create_struct(MPI_DATA_MSG_ITEMS_COUNT, blocklengths, offsets, types, &(this->_mpiDataMsgType));
 MPI_Type_commit(&(this->_mpiDataMsgType));
}















