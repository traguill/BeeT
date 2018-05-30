#ifndef __DEBUG_SAMPLE_RETURN_STATUS_H__
#define __DEBUG_SAMPLE_RETURN_STATUS_H__

#include "dSample.h"
#include "Data.h"
#include "../SharedData/SharedEnums.h"

class BTNode;

class dsReturnStatus : public dSample
{
public:
	dsReturnStatus(dBehaviorTree* bt, const Data& data);
	~dsReturnStatus();

	void Print()const;
	void Effect();
	void CounterEffect();

private:
	BTNode * node;
	NodeStatus oldStatus;
	NodeStatus newStatus;
};
#endif // !__DEBUG_SAMPLE_RETURN_STATUS_H__

