#ifndef __BEET_BEHAVIORTREE_H__
#define __BEET_BEHAVIORTREE_H__

#include "BeeT_serializer.h"
#include "BeeT_node.h"
#include "beet_std.h"
#include "BeeT_blackboard.h"

typedef struct BeeT_BehaviorTree BeeT_BehaviorTree;
struct BeeT_BehaviorTree
{
	unsigned int uid;
	BeeT_Node* rootNode;
	BeeT_Blackboard* bb;
	dequeue* runningNodes;

	void(*StartBehavior)(BeeT_BehaviorTree*, BeeT_Node*, ObserverFunc*);
	void(*StopBehavior)(BeeT_Node*, NodeStatus);
	void(*Update)(BeeT_BehaviorTree*);
	BEET_bool(*Step)(BeeT_BehaviorTree*);

};

BeeT_BehaviorTree* BeeT_BehaviorTree__Init(const BeeT_Serializer* data);
void BeeT_BehaviorTree__Destroy(BeeT_BehaviorTree* self);



#endif // !__BEET_BEHAVIORTREE_H__

