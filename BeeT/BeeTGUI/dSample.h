#ifndef __DEBUG_SAMPLE_H__
#define __DEBUG_SAMPLE_H__

#include "../SharedData/SharedEnums.h"

class dSample
{
public:
	dSample(SampleType type, double timestamp);
	~dSample();

	SampleType GetType()const;
	double GetTimestamp()const;

	virtual void Print()const;

protected:
	SampleType type;
	double timestamp;
};

#endif // !__DEBUG_SAMPLE_H__
