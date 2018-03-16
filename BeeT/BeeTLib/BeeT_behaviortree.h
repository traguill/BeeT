#ifndef __BEET_BEHAVIORTREE_H__
#define __BEET_BEHAVIORTREE_H__

#include "BeeT_serializer.h"

struct BeeT_Node;

struct BeeT_BehaviorTree
{
	int uid;
	BeeT_Node* rootNode = nullptr;

	BeeT_BehaviorTree(BeeT_Serializer& data);
	~BeeT_BehaviorTree();
};
#endif // !__BEET_BEHAVIORTREE_H__

