#include "BeeT_node.h"

BeeT_Node::BeeT_Node(BeeT_Serializer& data)
{
	id = data.GetInt("id");
	type = data.GetInt("type");
	int numChilds = data.GetArraySize("childs");
	for (int i = 0; i < numChilds; ++i)
	{
		BeeT_Node* child = new BeeT_Node(data.GetArray("childs", i));
		if (child)
			childs.push_back(child);
	}
}

BeeT_Node::~BeeT_Node()
{
	for (auto child : childs)
		delete child;
}