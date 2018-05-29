#ifndef __DEBUG_SAMPLE_BB_VAR_H__
#define __DEBUG_SAMPLE_BB_VAR_H__

#include "dSample.h"
#include "../SharedData/SharedEnums.h"
#include <string>

class dBehaviorTree;

class dsBBVar : public dSample
{
public:
	dsBBVar(dBehaviorTree* bt, SampleType type, double timestamp);
	~dsBBVar();

	void Print()const;
	void Effect();
	void CounterEffect();

public:
	std::string name;
	BBVarType varType;
	void* oldValue = nullptr;
	void* newValue = nullptr;
};
#endif // !__DEBUG_SAMPLE_BB_VAR_H__

