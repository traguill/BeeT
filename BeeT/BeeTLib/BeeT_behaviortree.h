#ifndef __BEET_BEHAVIORTREE_H__
#define __BEET_BEHAVIORTREE_H__

#include "BeeT_serializer.h"

class BeeT_BehaviorTree
{
public:
	BeeT_BehaviorTree(BeeT_Serializer& data);
	~BeeT_BehaviorTree();

public:
	int uid;
	int numNodes;
	int numLinks;
};
#endif // !__BEET_BEHAVIORTREE_H__

