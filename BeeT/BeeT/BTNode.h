#ifndef __BTNODE_H__
#define __BTNODE_H__

#include <vector>
#include <string>
#include "BTNodeTypes.h"
#include "ThirdParty/NodeEditor/Source/Shared/Math2D.h"

class BTLink;

const float padding = 12.0f; // Testing

class BTNode
{
public:
	BTNode(int id, int sourcePinId, int targetPinId, BTNodeType type, BTNode* parent = nullptr, BTLink* inputLink = nullptr);
	~BTNode();

	void PrepareToDraw();
	
	// Getters
	int GetId()const;
	std::string GetName()const;

public:
	ax::rect inputsRect;
	ax::rect contentRect;
	ax::rect outputsRect;

private:
	BTNode* parent = nullptr;
	std::vector<BTNode*> childs;
	int id = -1;
	BTNodeType type;

	BTLink* inputLink = nullptr; // Only one input link for node
	std::vector<BTLink*> outputLinks; // Some nodes have multiple child nodes, in consequence, multiple output links

	int sourcePinId = -1;
	int targetPinId = -1;

	std::string name;
	std::string comment;
};
#endif

