#ifndef __DEBUG_SAMPLE_NEW_CURRENT_H__
#define __DEBUG_SAMPLE_NEW_CURRENT_H__

#include "dSample.h"
#include "Data.h"

class BTNode;

class dsNewCurrent : public dSample
{
public:
	dsNewCurrent(dBehaviorTree* bt, const Data& data);
	~dsNewCurrent();

	void Print()const;
	void Effect();
	void CounterEffect();

private:
	BTNode * oldCurrent = nullptr;
	BTNode * newCurrent = nullptr;
};
#endif // !__DEBUG_SAMPLE_NEW_CURRENT_H__

