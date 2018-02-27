#ifndef __BTNODE_H__
#define __BTNODE_H__

#include <vector>
#include <string>
#include "BTNodeTypes.h"
#include "BTLink.h"
#include "BTPin.h"
#include "ThirdParty/NodeEditor/Source/Shared/Math2D.h"

const float padding = 12.0f; // Testing

class BTNode
{
public:
	BTNode(int id, int sourcePinId, int targetPinId, int typeId, BTNode* parent = nullptr);
	~BTNode();

	std::vector<BTLink*> ClearLinks();

	void PrepareToDraw();
	
	// Getters
	int GetId()const;
	std::string GetName()const;
	
public:
	ax::rect inputsRect;
	ax::rect contentRect;
	ax::rect outputsRect;

	BTPin* inputPin = nullptr;
	BTPin* outputPin = nullptr;

	const NodeType* type;

private:
	BTNode* parent = nullptr;
	std::vector<BTNode*> childs;
	int id = -1;

	std::string name;
	std::string comment;
};
#endif

