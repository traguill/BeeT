#include "BeeT_behaviortree.h"
#include "BeeT_node.h"
#include <stdio.h>
#include <stdlib.h>

BeeT_BehaviorTree* BeeT_BehaviorTree__Init(const BeeT_Serializer * data)
{
	BeeT_BehaviorTree* tree = malloc(sizeof(BeeT_BehaviorTree));
	tree->uid = BeeT_Serializer__GetInt(data, "uid");
	int rootId = BeeT_Serializer__GetInt(data, "rootId");
	int numNodes = BeeT_Serializer__GetArraySize(data, "nodes");
	for (int i = 0; i < numNodes; ++i)
	{
		BeeT_Serializer* nodeData = BeeT_Serializer__GetArray(data, "nodes", i);
		if (BeeT_Serializer__GetInt(nodeData, "id") == rootId)
		{
			BeeT_Serializer* rootNodeData = BeeT_Serializer__GetArray(data, "nodes", i);
			tree->rootNode = BeeT_Node__Init(rootNodeData);
			free(nodeData);
			free(rootNodeData);
			break;
		}
		free(nodeData);
	}
	return tree;
}

void BeeT_BehaviorTree__Destroy(BeeT_BehaviorTree * self)
{
	if (self->rootNode)
	{
		BeeT_Node__Destroy(self->rootNode);
		free(self);
	}
}
