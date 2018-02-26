#ifndef __BTNODE_H__
#define __BTNODE_H__

#include <vector>
#include <string>
#include "BTNodeTypes.h"
#include "BTLink.h"
#include "ThirdParty/NodeEditor/Source/Shared/Math2D.h"

const float padding = 12.0f; // Testing

class BTNode
{
public:
	BTNode(int id, int sourcePinId, int targetPinId, int typeId, BTNode* parent = nullptr);
	~BTNode();

	void PrepareToDraw();
	
	// Getters
	int GetId()const;
	std::string GetName()const;
	std::string GetTypeName()const;
	
	bool IsInputLinkAvailable()const; // Returns true if a link connection can be done
	bool IsOutputLinkAvailable()const; // Returns true if a link connection can be done
	void CreateInputLink();
	void CreateOutputLink();
	
public:
	ax::rect inputsRect;
	ax::rect contentRect;
	ax::rect outputsRect;

	BTPin inputPin;
	BTPin outputPin;

private:
	BTNode* parent = nullptr;
	std::vector<BTNode*> childs;
	int id = -1;
	const NodeType* type;

	std::string name;
	std::string comment;

	int numInputLinks = 0;
	int numOutputLinks = 0;
};
#endif

