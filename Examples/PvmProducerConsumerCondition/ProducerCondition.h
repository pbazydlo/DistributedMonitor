#ifndef _PRODUCER_CONDITION_
#define _PRODUCER_CONDITION_
#include "Common.h"

class ProducerCondition : public ICondition
{
	private:
		Buffer *_buf;
	public:
		ProducerCondition(Buffer *buffer);
		bool IsMet();
};

#endif
