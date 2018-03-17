#include "BeeT_node.h"
#include <stdio.h>
#include <stdlib.h>

BeeT_Node* BeeT_Node__Init(BeeT_Serializer* data)
{
	BeeT_Node* node = malloc(sizeof(BeeT_Node));
	node->id = BeeT_Serializer__GetInt(data, "id");
	node->type = BeeT_Serializer__GetInt(data, "type");
	node->numChilds = BeeT_Serializer__GetArraySize(data, "childs");
	int childsSize = sizeof(BeeT_Node*) * node->numChilds;
	node->childs = malloc(childsSize);
	for (int i = 0; i < node->numChilds; ++i)
	{
		node->childs[i] = malloc(sizeof(BeeT_Node));
		BeeT_Node__Init(node->childs[i], BeeT_Serializer__GetArray(data, "childs", i));
	}
	return node;
}

void BeeT_Node__Destroy(BeeT_Node * self)
{
	for (int i = 0; i < self->numChilds; ++i)
		free(self->childs[i]);
	free(self->childs);
	free(self);
}
