#include "BeeT_node.h"

BeeT_Node::BeeT_Node()
{}

BeeT_Node::~BeeT_Node()
{
	for (int i = 0; i < numChilds; ++i)
		delete childs[i];

	delete childs;
}

void InitNode(BeeT_Serializer & data)
{
	id = data.GetInt("id");
	type = data.GetInt("type");
	numChilds = data.GetArraySize("childs");
	childs = new BeeT_Node*[numChilds];
	for (int i = 0; i < numChilds; ++i)
	{
		childs[i] = new BeeT_Node();
		childs[i]->Init(data.GetArray("childs", i));
	}
}
