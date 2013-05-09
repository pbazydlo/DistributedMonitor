#include "Common.h"

void Buffer::Put(int element)
{
	this->Lock();

	while(counter>=CAPACITY) {
		this->Unlock();
		this->Lock();
	}
	
	this->counter++;
	this->elements[this->putPosition] = element;
	this->putPosition = (this->putPosition+1) % CAPACITY;
	std::cout << "Producer: Element " << element << " added\n";
	
	this->Unlock();
}

int Buffer::Get() {
	int output;

	this->Lock();

	while(counter<=0) {
		this->Unlock();
		this->Lock();
	}

	this->counter--;
	output = this->elements[this->getPosition];
	this->getPosition = (this->getPosition+1) % CAPACITY;
	std::cout << "Consumer: Element " << output << " taken\n";
	
	this->Unlock();

	return output;
}

char* Buffer::Serialize()
{
	char* data = new char[127];
	sprintf(data,"%d;%d;%d;%d;%d;%d;%d;%d",
		this->elements[0],
		this->elements[1],
		this->elements[2],
		this->elements[3],
		this->elements[4],
		this->putPosition,
		this->getPosition,
		this->counter);
	return data;
}

void Buffer::Deserialize(char* serializedContent)
{
	std::cout<<"Deserializing... got: "<<serializedContent<<std::endl;
	sscanf( serializedContent,"%d;%d;%d;%d;%d;%d;%d;%d",
		&(this->elements[0]),
		&(this->elements[1]),
		&(this->elements[2]),
		&(this->elements[3]),
		&(this->elements[4]),
		&(this->putPosition),
		&(this->getPosition),
		&(this->counter) );
}
