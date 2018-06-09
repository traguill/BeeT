#ifndef __DEBUG_SAMPLE_BEHAVIORTREE_END_H__
#define __DEBUG_SAMPLE_BEHAVIORTREE_END_H__

#include "dSample.h"
#include "Data.h"
#include "BTNode.h"
#include <vector>

struct BTNodePrevEnd
{
	BTNode* node;
	NodeColor prevColor;
};

class dsBTEnd : public dSample
{
public:
	dsBTEnd(dBehaviorTree* bt, const Data& data);
	~dsBTEnd();

	void Print()const;
	void Effect();
	void CounterEffect();

private:
	std::vector<BTNodePrevEnd*> treeNodes;
};
#endif // !__DEBUG_SAMPLE_BEHAVIORTREE_END_H__

