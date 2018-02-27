#ifndef __BEHAVIORTREE_H__
#define __BEHAVIORTREE_H__

#include <map>

class BTNode;
class BTLink;
class BTPin;

class BehaviorTree
{
public:
	BehaviorTree();
	~BehaviorTree();

	// Edition
	void AddNode(float posX, float posY, int typeId);
	void AddLink(BTPin* startPinId, BTPin* endPinId);

	void RemoveNode(int id);

	void Draw();

	BTNode* FindNode(int id)const;
	BTLink* FindLink(int id)const;
	BTPin* FindPin(int id)const;

private:
	int GetNextId();

private:
	std::map<int, BTNode*> nodesList;
	std::map<int, BTLink*> linksList;

	BTNode* rootNode = nullptr;

	int nextId = 0;

	// Draw Nodes configuration
	const float rounding = 5.0f;
};
#endif