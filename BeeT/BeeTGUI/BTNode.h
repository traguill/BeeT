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
class BTDecorator;
class Blackboard;
struct BBVar;

enum NodeColor
{
	NC_IDLE,
	NC_RUNNING,
	NC_SUCCESS,
	NC_FAILURE
};

class BTNode
{
public:
	BTNode();
	BTNode(int id, int sourcePinId, int targetPinId, int typeId, BehaviorTree* bt, BTNode* parent = nullptr);
	BTNode(BehaviorTree* bt, Data& data);
	~BTNode();

	virtual void Save(Data& file);
	void PreDrawSetColor();
	virtual void PrepareToDraw();

	// Inspector
	virtual void InspectorInfo();

	// Edition
	void AddDecorator(Blackboard* bb, BBVar* var);
	void RemoveDecorator(BTDecorator* dec);
	
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
	void MoveChild(int id, bool moveUp);

	// Utils
	void ForceRoot(); // Forces the node to be the root node. DO NOT USE!

protected:
	void ReloadSubtreeId(); // Sets the subtree id depending on the parent's

	void InitExtraData();
		
	// Inspector
	void InspectorComposite();
	void InspectorWait();
	
public:
	ax::rect inputsRect;
	ax::rect contentRect;
	ax::rect outputsRect;

	BTPin* inputPin = nullptr;
	BTPin* outputPin = nullptr;

	std::string name;
	const NodeType* type;

	bool saveFlag = false; // True when has been serialized
	
	std::vector<BTDecorator*> decorators; // Decorators attached to this node

	NodeColor nodeColor = NC_IDLE;
	bool highlightBorder = false;
protected:
	BTNode* parent = nullptr;
	std::vector<BTNode*> childs;
	int id = -1;

	BehaviorTree* bt = nullptr;

	int subtreeId = -1; // If this is 0 means that this node is inside the Root tree and will be executed. 
	std::vector<BTDecorator*> decoratorsToRemove;

	char* extraData = nullptr;
};
#endif

