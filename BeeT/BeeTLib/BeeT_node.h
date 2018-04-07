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

typedef enum NodeType // Careful! This types need to be sync with the ones in the file 'BTNodeTypes.cpp -> Init()'
{
	NT_ROOT,
	NT_SELECTOR,
	NT_SEQUENCE,
	NT_PARALLEL,
	NT_TASK
}NodeType;

typedef void(*ObserverFunc)(NodeStatus);

typedef struct BeeT_Node{
	int id;
	NodeType type;

	NodeStatus status;
	ObserverFunc observer;

	NodeStatus(*Tick)(BeeT_Node*);
	NodeStatus(*Update)(BeeT_Node*);
	
	void(*OnInit)();
	void(*OnFinish)(NodeStatus);
}BeeT_Node;

BeeT_Node* BeeT_Node__Init(const BeeT_Serializer* data);
void BeeT_Node__Destroy(BeeT_Node* self);


// ---------------------------------------------------------------------------------
// Basic Nodes
// ---------------------------------------------------------------------------------
typedef struct BTN_Root
{
	BeeT_Node* node;
}BTN_Root;


typedef struct BTN_Composite
{
	BeeT_Node* node;
	struct BeeT_Node** childs;
	int numChilds;
}BTN_Composite;

typedef BTN_Composite BTN_Selector;
typedef BTN_Composite BTN_Sequence;

// Parallel TODO

typedef struct BTN_Task
{
	BeeT_Node* node;
	//data?
}BTN_Task;
#endif // !__BEET_NODE_H__

