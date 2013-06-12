#include "ProducerCondition.h"

ProducerCondition::ProducerCondition(Buffer *buffer)
{
	this->_buf = buffer;
}

bool ProducerCondition::IsMet()
{
	return this->_buf->GetCounter() < CAPACITY;
}
