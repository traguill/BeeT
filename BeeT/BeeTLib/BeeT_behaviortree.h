#ifndef __BEET_BEHAVIORTREE_H__
#define __BEET_BEHAVIORTREE_H__

#include "beet.h"

#include "BeeT_serializer.h"
#include "BeeT_node.h"
#include "beet_std.h"

typedef struct BeeT_BehaviorTree BeeT_BehaviorTree;
struct BeeT_BehaviorTree
{
	unsigned int uid;
	BeeT_Node* rootNode;

	dequeue* runningNodes;
	beetCallbackFunc callbackFunc;

	void(*StartBehavior)(BeeT_BehaviorTree*, BeeT_Node*, ObserverFunc*);
	void(*StopBehavior)(BeeT_Node*, NodeStatus);
	void(*Update)(BeeT_BehaviorTree*);
	BEET_bool(*Step)(BeeT_BehaviorTree*);

};

BeeT_BehaviorTree* BeeT_BehaviorTree__Init(const BeeT_Serializer* data, beetCallbackFunc callbackFunc);
void BeeT_BehaviorTree__Destroy(BeeT_BehaviorTree* self);



#endif // !__BEET_BEHAVIORTREE_H__

