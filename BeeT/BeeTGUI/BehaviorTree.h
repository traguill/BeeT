#ifndef __BEHAVIORTREE_H__
#define __BEHAVIORTREE_H__

#include <map>
#include "Data.h"

class BTNode;
class BTLink;
class BTPin;

class BehaviorTree
{
public:
	BehaviorTree();
	BehaviorTree(Data& data);
	~BehaviorTree();

	// Edition
	int AddNode(float posX, float posY, int typeId);
	void AddLink(BTPin* startPinId, BTPin* endPinId);

	void RemoveNode(int id);
	void RemoveLink(int id);

	// Serialization
	int Serialize(char** buffer)const;

	void Draw();

	BTNode* FindNode(int id)const;
	BTLink* FindLink(int id)const;
	BTPin* FindPin(int id)const;
	bool IsRoot(int id)const;

private:
	int GetNextId();

	// Serialization
	void AddNode(Data& data, std::map<int, BTPin*>& pinList);

private:
	unsigned int uid = 0;
	std::map<int, BTNode*> nodesList;
	std::map<int, BTLink*> linksList;

	BTNode* rootNode = nullptr;

	int nextId = 0;

	// Draw Nodes configuration
	const float rounding = 5.0f;
};
#endif