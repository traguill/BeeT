#include "BeeT_behaviortree.h"
#include "BeeT_node.h"

BeeT_BehaviorTree::BeeT_BehaviorTree(BeeT_Serializer& data)
{
	uid = data.GetInt("uid");

	int numNodes = data.GetArraySize("nodes");
	for (int i = 0; i < numNodes; ++i)
	{
		BeeT_Node* node = new BeeT_Node(data.GetArray("nodes", i));
		if (node != NULL)
		{
			// TODO: save node in structure
		}
	}
}

BeeT_BehaviorTree::~BeeT_BehaviorTree()
{
}