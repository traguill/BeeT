#ifndef __BEET_NODE_H__
#define __BEET_NODE_H__

#include "BeeT_serializer.h"
#include "BeeT_std.h"

// Forward declarations
typedef struct BeeT_BehaviorTree;

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

typedef NodeStatus(*beetCallbackFunc)(unsigned int btId, const char* taskId);
typedef struct BTN_Task
{
	BeeT_Node node;
	const char* name;
	beetCallbackFunc callbackFunc;
}BTN_Task;

// ---------------------------------------------------------------------------------
// Node functions
// ---------------------------------------------------------------------------------

NodeStatus Tick(BeeT_Node* n);

// Inits Node Types
BTN_Root*		BTN_Root_Init(const BeeT_Serializer* data,struct BeeT_BehaviorTree* bt);
BTN_Selector*	BTN_Selector_Init(const BeeT_Serializer* data, struct BeeT_BehaviorTree* bt);
BTN_Sequence*	BTN_Sequence_Init(const BeeT_Serializer* data, struct BeeT_BehaviorTree* bt);
// Parallel TODO: BTN_Root* BTN_Root_Init(BeeT_Node* n, const BeeT_Serializer* data);
BTN_Task*		BTN_Task_Init(const BeeT_Serializer* data);

// OnInit functions
void BTN_Root_OnInit(BeeT_Node* self);
void BTN_Selector_OnInit(BeeT_Node* self);
void BTN_Sequence_OnInit(BeeT_Node* self);
void BTN_Task_OnInit(BeeT_Node* self);

// Update functions
NodeStatus BTN_Root_Update(BeeT_Node* self);
NodeStatus BTN_Selector_Update(BeeT_Node* self);
NodeStatus BTN_Sequence_Update(BeeT_Node* self);
NodeStatus BTN_Task_Update(BeeT_Node* self);

// OnFinish functions
void BTN_Root_OnFinish(BeeT_Node* self, NodeStatus status);
void BTN_Selector_OnFinish(BeeT_Node* self, NodeStatus status);
void BTN_Sequence_OnFinish(BeeT_Node* self, NodeStatus status);
void BTN_Task_OnFinish(BeeT_Node* self, NodeStatus status);

// OnDestroy functions
void BTN_Root_OnDestroy(BTN_Root* self);
void BTN_Composite_OnDestroy(BTN_Composite* self);
void BTN_Task_OnDestroy(BTN_Task* self);

// Observer functions
void BTN_Root_TreeFinish(BeeT_Node* self, NodeStatus s);
void BTN_Selector_OnChildFinish(BeeT_Node* self, NodeStatus s);
void BTN_Sequence_OnChildFinish(BeeT_Node* self, NodeStatus s);

// ---------------------------------------------------------------------------------
#endif // !__BEET_NODE_H__

