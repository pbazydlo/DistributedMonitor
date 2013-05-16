#include "Common.h"

void ReadersWriters::WantsToRead()
{
	this->Lock();

	while(writersCount>0) {
		this->Unlock();
		this->Lock();
	}
	
	this->readersCount++;
	std::cout << "Reader #" << readersCount << " is reading\n";
	
	this->Unlock();
}

void ReadersWriters::FinishReading() 
{
	this->Lock();
	this->readersCount--;
	std::cout << "Reader finished reading\n";
	this->Unlock();
}

void ReadersWriters::WantsToWrite() 
{
	this->Lock();

	while(readersCount>0) {
		this->Unlock();
		this->Lock();
	}

	this->writersCount++;
	std::cout << "Writer is writing\n";
	
	this->Unlock();
}

void ReadersWriters::FinishWriting() 
{
	this->Lock();
	this->writersCount--;
	std::cout << "Writer finished writing\n";
	this->Unlock();
}

char* ReadersWriters::Serialize()
{
	char* data = new char[127];
	sprintf(data,"%d;%d",
		this->readersCount,
		this->writersCount);
	return data;
}

void ReadersWriters::Deserialize(char* serializedContent)
{
	std::cout<<"Deserializing... got: "<<serializedContent<<std::endl;
	sscanf( serializedContent,"%d;%d",
		&(this->readersCount),
		&(this->writersCount) );
}
