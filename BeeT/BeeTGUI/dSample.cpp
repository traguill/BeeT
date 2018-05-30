#include "dSample.h"
#include "dBehaviorTree.h"

dSample::dSample(dBehaviorTree* bt, SampleType type) : bt(bt), type(type)
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
