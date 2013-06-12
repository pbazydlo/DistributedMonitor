#ifndef _CONSUMER_CONDITION_
#define _CONSUMER_CONDITION_
#include "Common.h"

class ConsumerCondition : public ICondition
{
	private:
		Buffer *_buf;
	public:
		ConsumerCondition(Buffer *buffer);
		bool IsMet();
};

#endif
