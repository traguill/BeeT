#ifndef __DEBUG_SAMPLE_DECORATOR_CONDITION_H__
#define __DEBUG_SAMPLE_DECORATOR_CONDITION_H__

#include "dSample.h"
#include "Data.h"

class BTDecorator;

class dsDecoratorCondition : public dSample
{
public:
	dsDecoratorCondition(dBehaviorTree* bt, const Data& data);
	~dsDecoratorCondition();

	void Print()const;
	void Effect();
	void CounterEffect();

private:
	bool pass = false;
	BTDecorator* decorator = nullptr;
};
#endif // !__DEBUG_SAMPLE_DECORATOR_CONDITION_H__

