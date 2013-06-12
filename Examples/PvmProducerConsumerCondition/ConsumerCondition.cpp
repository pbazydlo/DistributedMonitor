#include "ConsumerCondition.h"

ConsumerCondition::ConsumerCondition(Buffer *buffer)
{
	this->_buf = buffer;
}

bool ConsumerCondition::IsMet()
{
	return this->_buf->GetCounter() > 0;
}
