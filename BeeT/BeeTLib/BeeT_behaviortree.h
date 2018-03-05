#ifndef __BEET_BEHAVIORTREE_H__
#define __BEET_BEHAVIORTREE_H__

#include "BeeT_serializer.h"

class BeeT_Node;

class BeeT_BehaviorTree
{
public:
	BeeT_BehaviorTree(BeeT_Serializer& data);
	~BeeT_BehaviorTree();

public:
	int uid;
	BeeT_Node* rootNode = nullptr;
};
#endif // !__BEET_BEHAVIORTREE_H__

