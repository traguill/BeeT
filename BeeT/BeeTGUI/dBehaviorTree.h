#ifndef __DEBUG_BEHAVIOR_TREE_H__
#define __DEBUG_BEHAVIOR_TREE_H__

#include "Data.h"
#include "BehaviorTree.h"
#include <vector>

class dSample;

class dBehaviorTree : public BehaviorTree
{
public:
	dBehaviorTree();
	dBehaviorTree(Data& data);
	~dBehaviorTree();

	void AddSample(dSample* sample);
	void PrintSamples();

	void ApplySampleEffect(int newSample);

public:
	int debugUID = -1;
	int sampleSelected = -1;
	std::vector<dSample*> changes;
};
#endif // !__DEBUG_BEHAVIOR_TREE_H__

