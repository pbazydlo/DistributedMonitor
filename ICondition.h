#ifndef _CONDITION_
#define _CONDITION_

class ICondition
{
	public:
		ICondition();
		virtual bool IsMet() = 0;
};

#endif
