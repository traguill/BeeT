#include "dSample.h"
#include "dBehaviorTree.h"

dSample::dSample(dBehaviorTree* bt, SampleType type, double timestamp) : bt(bt), type(type), timestamp(timestamp)
{
}

dSample::~dSample()
{
}

SampleType dSample::GetType() const
{
	return type;
}

double dSample::GetTimestamp() const
{
	return timestamp;
}

void dSample::Print() const
{
}

void dSample::Effect()
{
}

void dSample::CounterEffect()
{
}
