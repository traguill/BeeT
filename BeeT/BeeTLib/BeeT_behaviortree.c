#include "BeeT_behaviortree.h"
#include "BeeT_node.h"

BeeT_BehaviorTree::BeeT_BehaviorTree(BeeT_Serializer& data)
{
	uid = data.GetInt("uid");
	int rootId = data.GetInt("rootId");
	int numNodes = data.GetArraySize("nodes");
	for (int i = 0; i < numNodes; ++i)
	{
		BeeT_Serializer nodeData = data.GetArray("nodes", i);
		if (nodeData.GetInt("id") == rootId)
		{
			rootNode = new BeeT_Node();
			rootNode->Init(data.GetArray("nodes", i));
			break;
		}
	}
}

BeeT_BehaviorTree::~BeeT_BehaviorTree()
{
	if (rootNode)
		delete rootNode;
}