#include "BeeT_node.h"
#include <stdio.h>
#include <stdlib.h>

BeeT_Node* BeeT_Node__Init(const BeeT_Serializer* data)
{
	BeeT_Node* node = BEET_malloc(sizeof(BeeT_Node));
	node->id = BeeT_Serializer__GetInt(data, "id");
	node->type = BeeT_Serializer__GetInt(data, "type");
	node->numChilds = BeeT_Serializer__GetArraySize(data, "childs");
	int childsSize = sizeof(BeeT_Node*) * node->numChilds;
	node->childs = BEET_malloc(childsSize);
	for (int i = 0; i < node->numChilds; ++i)
	{
		node->childs[i] = BeeT_Node__Init(BeeT_Serializer__GetArray(data, "childs", i));
	}
	return node;
}

void BeeT_Node__Destroy(BeeT_Node * self)
{
	for (int i = 0; i < self->numChilds; ++i)
		BEET_free(self->childs[i]);
	BEET_free(self->childs);
	BEET_free(self);
}
