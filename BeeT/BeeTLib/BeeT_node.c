#include "BeeT_node.h"
#include "BeeT_behaviortree.h"
#include <stdio.h>

// ---------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------

NodeStatus Tick(BeeT_Node* n);

// Inits Node Types
BTN_Root*		BTN_Root_Init		(const BeeT_Serializer* data, BeeT_BehaviorTree* bt);
BTN_Selector*	BTN_Selector_Init	(const BeeT_Serializer* data, BeeT_BehaviorTree* bt);
BTN_Sequence*	BTN_Sequence_Init	(const BeeT_Serializer* data, BeeT_BehaviorTree* bt);
// Parallel TODO: BTN_Root* BTN_Root_Init(BeeT_Node* n, const BeeT_Serializer* data);
BTN_Task*		BTN_Task_Init		(const BeeT_Serializer* data);

// OnInit functions
void BTN_Root_OnInit		(BeeT_Node* self);
void BTN_Selector_OnInit	(BeeT_Node* self);
void BTN_Sequence_OnInit	(BeeT_Node* self);
void BTN_Task_OnInit		(BeeT_Node* self);

// Update functions
NodeStatus BTN_Root_Update		(BeeT_Node* self);
NodeStatus BTN_Selector_Update	(BeeT_Node* self);
NodeStatus BTN_Sequence_Update	(BeeT_Node* self);
NodeStatus BTN_Task_Update		(BeeT_Node* self);

// OnFinish functions
void BTN_Root_OnFinish		(BeeT_Node* self, NodeStatus status);
void BTN_Selector_OnFinish	(BeeT_Node* self, NodeStatus status);
void BTN_Sequence_OnFinish	(BeeT_Node* self, NodeStatus status);
void BTN_Task_OnFinish		(BeeT_Node* self, NodeStatus status);

// Observer functions
void BTN_Root_TreeFinish		(BeeT_Node* self, NodeStatus s);
void BTN_Selector_OnChildFinish	(BeeT_Node* self, NodeStatus s);
void BTN_Sequence_OnChildFinish (BeeT_Node* self, NodeStatus s);

// ---------------------------------------------------------------------------------

BeeT_Node* BeeT_Node__Init(const BeeT_Serializer* data, BeeT_BehaviorTree* bt)
{
	
	BeeT_Node* node = NULL;
	NodeType type = (NodeType)BeeT_Serializer__GetInt(data, "type");
	switch (type)
	{
	case NT_ROOT:
		node = (BeeT_Node*)BTN_Root_Init(data, bt);
		break;
	case NT_SELECTOR:
		node = (BeeT_Node*)BTN_Selector_Init(data, bt);
		break;
	case NT_SEQUENCE:
		node = (BeeT_Node*)BTN_Sequence_Init(data, bt);
		break;
	case NT_PARALLEL:
		break;
	case NT_TASK:
		node = (BeeT_Node*)BTN_Task_Init(data);
		break;
	}
	node->id = BeeT_Serializer__GetInt(data, "id");
	node->type = (NodeType)BeeT_Serializer__GetInt(data, "type");
	node->bt = bt;
	node->observer = node->observerNode = NULL;
	node->status = NS_INVALID;
	node->Tick = &Tick;
	return node;
}

void BeeT_Node__Destroy(BeeT_Node * self)
{
	/*for (int i = 0; i < self->numChilds; ++i)
		BEET_free(self->childs[i]);
	BEET_free(self->childs);*/
	BEET_free(self);
}

NodeStatus Tick(BeeT_Node* n)
{
	if (n->status == NS_INVALID)
	{
		n->OnInit(n);
		n->status = NS_RUNNING;
	}

	n->Update(n);

	if (n->status != NS_RUNNING)
	{
		n->OnFinish(n, n->status);
	}
	return n->status;
}


// BeeT Nodes Initialization by Type ---------------------------------------------

BTN_Root* BTN_Root_Init(const BeeT_Serializer* data, BeeT_BehaviorTree* bt)
{
	BTN_Root* btn = (BTN_Root*)BEET_malloc(sizeof(BTN_Root));

	int size = BeeT_Serializer__GetArraySize(data, "childs");
	if (BeeT_Serializer__GetArraySize(data, "childs") != 0)
	{
		btn->startNode = BeeT_Node__Init(BeeT_Serializer__GetArray(data, "childs", 0), bt);
	}

	btn->node.OnInit = &BTN_Root_OnInit;
	btn->node.Update = &BTN_Root_Update;
	btn->node.OnFinish = &BTN_Root_OnFinish;
	return btn;
}

BTN_Selector * BTN_Selector_Init(const BeeT_Serializer * data, BeeT_BehaviorTree* bt)
{
	BTN_Selector* btn = (BTN_Selector*)BEET_malloc(sizeof(BTN_Selector));
	
	btn->childs = InitDequeue();
	unsigned int numChilds = BeeT_Serializer__GetArraySize(data, "childs");
	for (int i = 0; i < numChilds; ++i)
	{
		BeeT_Node* child = BeeT_Node__Init(BeeT_Serializer__GetArray(data, "childs", i), bt);
		dequeue_push_back(btn->childs, child);
	}

	btn->node.OnInit = &BTN_Selector_OnInit;
	btn->node.Update = &BTN_Selector_Update;
	btn->node.observer = &BTN_Selector_OnFinish;
	return btn;
}

BTN_Sequence * BTN_Sequence_Init(const BeeT_Serializer * data, BeeT_BehaviorTree* bt)
{
	BTN_Sequence* btn = (BTN_Sequence*)BEET_malloc(sizeof(BTN_Sequence));

	btn->childs = InitDequeue();
	unsigned int numChilds = BeeT_Serializer__GetArraySize(data, "childs");
	for (int i = 0; i < numChilds; ++i)
	{
		BeeT_Node* child = BeeT_Node__Init(BeeT_Serializer__GetArray(data, "childs", i), bt);
		dequeue_push_back(btn->childs, child);
	}

	btn->node.OnInit = &BTN_Sequence_OnInit;
	btn->node.Update = &BTN_Sequence_Update;
	btn->node.OnFinish = &BTN_Sequence_OnFinish;
	return btn;
}

BTN_Task * BTN_Task_Init(const BeeT_Serializer * data)
{
	BTN_Task* btn = (BTN_Task*)BEET_malloc(sizeof(BTN_Task));

	const char* name = BeeT_Serializer__GetString(data, "name");
	unsigned int length = strlen(name) + 1;
	btn->name = (const char*)BEET_malloc(length);
	strcpy(btn->name, name);

	btn->node.OnInit = &BTN_Task_OnInit;
	btn->node.Update = &BTN_Task_Update;
	btn->node.OnFinish = &BTN_Task_OnFinish;
	return btn;
}

// BeeT Node OnInit functions -------------------------------------------------------------

void BTN_Root_OnInit(BeeT_Node* self)
{
	BTN_Root* btn = (BTN_Root*)self;
	btn->startNode->observerNode = btn;
	btn->node.bt->StartBehavior(btn->node.bt, btn->startNode, &BTN_Root_TreeFinish);
}

void BTN_Selector_OnInit(BeeT_Node* self)
{
	BTN_Selector* btn = (BTN_Selector*)self;
	btn->current = dequeue_head(btn->childs);
	BeeT_Node* current_node = (BeeT_Node*)dequeue_front(btn->childs);
	current_node->observerNode = btn;
	btn->node.bt->StartBehavior(btn->node.bt, current_node, &BTN_Selector_OnChildFinish);
}

void BTN_Sequence_OnInit(BeeT_Node* self)
{
	BTN_Sequence* btn = (BTN_Sequence*)self;
	btn->current = dequeue_head(btn->childs);
	BeeT_Node* current_node = (BeeT_Node*)dequeue_front(btn->childs);
	current_node->observerNode = btn;
	btn->node.bt->StartBehavior(btn->node.bt, current_node, &BTN_Sequence_OnChildFinish);
}

void BTN_Task_OnInit(BeeT_Node* self)
{}


// BeeT Node Update functions --------------------------------------------------------------

NodeStatus BTN_Root_Update(BeeT_Node* self)
{
	return self->status = NS_RUNNING;
}

NodeStatus BTN_Selector_Update(BeeT_Node* self)
{
	return self->status = NS_RUNNING;
}

NodeStatus BTN_Sequence_Update(BeeT_Node* self)
{
	return self->status = NS_RUNNING;
}

NodeStatus BTN_Task_Update(BeeT_Node* self)
{
	BTN_Task* btn = (BTN_Task*)self;
	printf("Node(%d): %s\n", btn->node.id, btn->name);
	return self->status = NS_SUCCESS;
}


// BeeT Node OnFinish functions -------------------------------------------------------------

void BTN_Root_OnFinish(BeeT_Node* self, NodeStatus status)
{
}

void BTN_Selector_OnFinish(BeeT_Node* self, NodeStatus status)
{
}

void BTN_Sequence_OnFinish(BeeT_Node* self, NodeStatus status)
{
}

void BTN_Task_OnFinish(BeeT_Node* self, NodeStatus status)
{
}

// BeeT Node Observer functions -------------------------------------------------------------

void BTN_Root_TreeFinish(BeeT_Node * self, NodeStatus s)
{
	self->bt->StopBehavior(self, s);
}

void BTN_Selector_OnChildFinish(BeeT_Node* self, NodeStatus s)
{

}

void BTN_Sequence_OnChildFinish(BeeT_Node* self, NodeStatus s)
{
	BTN_Sequence* btn = (BTN_Sequence*)self;
	BeeT_Node* child = (BeeT_Node*)btn->current->data;
	if (child->status == NS_FAILURE)
	{
		btn->node.bt->StopBehavior(btn, NS_FAILURE);
		return;
	}

	BEET_ASSERT(child->status == NS_SUCCESS);
	btn->current = btn->current->next;
	if (btn->current == NULL)
	{
		btn->node.bt->StopBehavior(btn, NS_SUCCESS);
	}
	else
	{
		BeeT_Node* n_current = (BeeT_Node*)btn->current->data;
		n_current->observerNode = btn;
		btn->node.bt->StartBehavior(btn->node.bt, n_current, &BTN_Sequence_OnChildFinish);
	}
}