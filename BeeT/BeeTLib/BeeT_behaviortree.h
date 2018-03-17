#ifndef __BEET_BEHAVIORTREE_H__
#define __BEET_BEHAVIORTREE_H__

#include "BeeT_serializer.h"

typedef struct
{
	int uid;
	struct BeeT_Node* rootNode;
}BeeT_BehaviorTree;

BeeT_BehaviorTree* BeeT_BehaviorTree__Init(const BeeT_Serializer* data);
void BeeT_BehaviorTree__Destroy(struct BeeT_BehaviorTree* self);

#endif // !__BEET_BEHAVIORTREE_H__

