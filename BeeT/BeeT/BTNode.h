#ifndef __BTNODE_H__
#define __BTNODE_H__

#include <vector>
#include <string>
#include "BTNodeTypes.h"

class BTLink;

class BTNode
{
public:
	BTNode();
	~BTNode();

private:
	BTNode* parent = nullptr;
	std::vector<BTNode*> childs;
	int id = -1;
	BTNodeType type;

	BTLink* inputLink = nullptr; // Only one input link for node
	std::vector<BTLink*> outputLinks; // Some nodes have multiple child nodes, in consequence, multiple output links

	std::string name;
	std::string comment;
};
#endif

