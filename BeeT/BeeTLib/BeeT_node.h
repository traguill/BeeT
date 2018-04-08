#ifndef __BEET_NODE_H__
#define __BEET_NODE_H__

#include "BeeT_serializer.h"
#include "BeeT_std.h"

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

typedef struct BeeT_Node BeeT_Node;
typedef void(*ObserverFunc)(BeeT_Node*, NodeStatus);
struct BeeT_Node{
	int id;
	NodeType type;
	struct BeeT_BehaviorTree* bt;

	NodeStatus status;
	ObserverFunc observer;
	BeeT_Node* observerNode;

	NodeStatus(*Tick)(BeeT_Node*);	// Do not rewrite in child classes

	void(*OnInit)(BeeT_Node*);
	NodeStatus(*Update)(BeeT_Node*);
	void(*OnFinish)(BeeT_Node*, NodeStatus);
};

BeeT_Node* BeeT_Node__Init(const BeeT_Serializer* data, struct BeeT_BehaviorTree* bt);
void BeeT_Node__Destroy(BeeT_Node* self);


// ---------------------------------------------------------------------------------
// Basic Nodes
// ---------------------------------------------------------------------------------
typedef struct BTN_Root
{
	BeeT_Node node;
	BeeT_Node* startNode;
}BTN_Root;


typedef struct BTN_Composite
{
	BeeT_Node node;
	dequeue* childs;
	node_deq* current;
}BTN_Composite;

typedef BTN_Composite BTN_Selector;
typedef BTN_Composite BTN_Sequence;

// Parallel TODO

typedef struct BTN_Task
{
	BeeT_Node node;
	const char* name;
}BTN_Task;
#endif // !__BEET_NODE_H__

