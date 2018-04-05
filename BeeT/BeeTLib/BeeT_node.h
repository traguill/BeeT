#ifndef __BEET_NODE_H__
#define __BEET_NODE_H__

#include "BeeT_serializer.h"

typedef enum NodeStatus
{
		NS_INVALID,
		NS_SUCCESS,
		NS_FAILURE,
		NS_RUNNING,
		NS_SUSPENDED
}NodeStatus;

typedef void(*ObserverFunc)(NodeStatus);

typedef struct BeeT_Node{
	int id;
	struct BeeT_Node** childs;
	int numChilds;
	int type; // Change for a real type

	NodeStatus status;
	ObserverFunc observer;
}BeeT_Node;

BeeT_Node* BeeT_Node__Init(const BeeT_Serializer* data);
void BeeT_Node__Destroy(BeeT_Node* self);

#endif // !__BEET_NODE_H__

