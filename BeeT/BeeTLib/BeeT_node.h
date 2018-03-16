#ifndef __BEET_NODE_H__
#define __BEET_NODE_H__

#include "BeeT_serializer.h"

struct BeeT_Node{
	int id;
	struct BeeT_Node** childs;
	int numChilds;
	int type; // Change for a real type
};

void InitNode(BeeT_Serializer& data);

#endif // !__BEET_NODE_H__

