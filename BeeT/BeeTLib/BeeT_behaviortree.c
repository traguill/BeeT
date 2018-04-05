#include "BeeT_behaviortree.h"
#include <stdio.h>
#include <stdlib.h>

BeeT_BehaviorTree* BeeT_BehaviorTree__Init(const BeeT_Serializer * data)
{
	BeeT_BehaviorTree* tree = BEET_malloc(sizeof(BeeT_BehaviorTree));
	tree->uid = BeeT_Serializer__GetUInt(data, "uid");
	int rootId = BeeT_Serializer__GetInt(data, "rootId");
	int numNodes = BeeT_Serializer__GetArraySize(data, "nodes");
	for (int i = 0; i < numNodes; ++i)
	{
		BeeT_Serializer* nodeData = BeeT_Serializer__GetArray(data, "nodes", i);
		if (BeeT_Serializer__GetInt(nodeData, "id") == rootId)
		{
			BeeT_Serializer* rootNodeData = BeeT_Serializer__GetArray(data, "nodes", i);
			tree->rootNode = BeeT_Node__Init(rootNodeData);
			BEET_free(nodeData);
			BEET_free(rootNodeData);
			break;
		}
		BEET_free(nodeData);
	}

	tree->StartBehavior = StartBehavior;
	tree->StopBehavior = StopBehavior;
	tree->Update = Update;
	tree->Step = Step;

	return tree;
}

void BeeT_BehaviorTree__Destroy(BeeT_BehaviorTree * self)
{
	if (self->rootNode)
	{
		BeeT_Node__Destroy(self->rootNode);
		BEET_free(self);
	}
}


// Functions -----------------------------------------------------------------------------
void StartBehavior(BeeT_BehaviorTree* bt, BeeT_Node* behavior, ObserverFunc* obsFunc)
{
	if (obsFunc != NULL)
		behavior->observer = obsFunc;


}
void StopBehavior(BeeT_BehaviorTree* bt, BeeT_Node* behavior, NodeStatus resultStatus)
{

}
void Update(BeeT_BehaviorTree* bt)
{

}
BEET_bool Step(BeeT_BehaviorTree* bt)
{
	return BEET_TRUE;
}