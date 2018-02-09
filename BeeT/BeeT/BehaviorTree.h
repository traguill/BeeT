#ifndef __BEHAVIORTREE_H__
#define __BEHAVIORTREE_H__

#include <map>

class BTNode;
class BTLink;

class BehaviorTree
{
public:
	BehaviorTree();
	~BehaviorTree();

private:
	std::map<int, BTNode*> nodesList;
	std::map<int, BTLink*> linksList;

	BTNode* rootNode = nullptr;
};
#endif