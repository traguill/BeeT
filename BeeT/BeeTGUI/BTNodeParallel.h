#ifndef __BEET_NODE_PARALLEL_H__
#define __BEET_NODE_PARALLEL_H__

#include "BTNode.h"

class BTNodeParallel : public BTNode
{
public:
	BTNodeParallel(int id, int sourcePinId, int targetPinId, int targetPin2Id, BehaviorTree* bt, BTNode* parent = nullptr);
	BTNodeParallel(BehaviorTree* bt, Data& data);
	~BTNodeParallel();

	void Save(Data& file);
	void InspectorInfo();
	void PrepareToDraw();

public:
	ax::rect simpleOutputsRect;
	BTPin* simplePin;

};
#endif // !__BEET_NODE_PARALLEL_H__

