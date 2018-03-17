#ifndef __BTNODE_H__
#define __BTNODE_H__

#include <vector>
#include <string>
#include "BTNodeTypes.h"
#include "BTLink.h"
#include "BTPin.h"
#include "ThirdParty/NodeEditor/Source/Shared/Math2D.h"
#include "Data.h"

const float padding = 12.0f; // Testing

class BehaviorTree;

class BTNode
{
public:
	BTNode(int id, int sourcePinId, int targetPinId, int typeId, BehaviorTree* bt, BTNode* parent = nullptr);
	BTNode(BehaviorTree* bt, Data& data);
	~BTNode();

	void Save(Data& file);
	void PrepareToDraw();
	
	// Getters
	int GetId()const;
	BTNode* GetParent()const;
	std::vector<BTNode*> GetChilds()const;	// Use for Debug
	std::vector<BTLink*> GetAllLinks();
	int GetSubtreeId()const;

	// Hierarchy
	void SetParent(BTNode* parent);
	void RemoveParent();
	void AddChild(BTNode* child);
	void RemoveChild(BTNode* child);

	// Utils
	void ForceRoot(); // Forces the node to be the root node. DO NOT USE!

private:
	void ReloadSubtreeId(); // Sets the subtree id depending on the parent's
	
public:
	ax::rect inputsRect;
	ax::rect contentRect;
	ax::rect outputsRect;

	BTPin* inputPin = nullptr;
	BTPin* outputPin = nullptr;

	std::string name;
	const NodeType* type;

	bool saveFlag = false; // True when has been serialized

private:
	BTNode* parent = nullptr;
	std::vector<BTNode*> childs;
	int id = -1;

	std::string comment;
	BehaviorTree* bt = nullptr;

	int subtreeId = -1; // If this is 0 means that this node is inside the Root tree and will be executed. 
};
#endif

