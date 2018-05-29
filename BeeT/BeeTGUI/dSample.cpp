#include "dSample.h"

dSample::dSample(SampleType type, double timestamp) : type(type), timestamp(timestamp)
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
