#ifndef __DEBUG_SAMPLE_H__
#define __DEBUG_SAMPLE_H__

#include "../SharedData/SharedEnums.h"

class dBehaviorTree;

class dSample
{
public:
	dSample(dBehaviorTree* bt, SampleType type);
	~dSample();

	SampleType GetType()const;
	float GetTimestamp()const;

	virtual void Print()const;
	virtual void Effect();			// Modifies the BT applying this sample change
	virtual void CounterEffect();	// Modifies the BT applying the inverse effect of this sample change.

protected:
	dBehaviorTree* bt;
	SampleType type;
	float timestamp;
};

#endif // !__DEBUG_SAMPLE_H__
