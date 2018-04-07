#include "BeeT_node.h"
#include <stdio.h>
#include <stdlib.h>

// ---------------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------------

NodeStatus Tick(BeeT_Node* n);

// Inits Node Types
BTN_Root*		BTN_Root_Init		(BeeT_Node* n, const BeeT_Serializer* data);
BTN_Selector*	BTN_Selector_Init	(BeeT_Node* n, const BeeT_Serializer* data);
BTN_Sequence*	BTN_Sequence_Init	(BeeT_Node* n, const BeeT_Serializer* data);
// Parallel TODO: BTN_Root* BTN_Root_Init(BeeT_Node* n, const BeeT_Serializer* data);
BTN_Task*		BTN_Task_Init		(BeeT_Node* n, const BeeT_Serializer* data);

// Function OnInit Node Types
void BTN_Root_OnInit();
void BTN_Selector_OnInit();
void BTN_Sequence_OnInit();
void BTN_Task_OnInit();

// ---------------------------------------------------------------------------------

BeeT_Node* BeeT_Node__Init(const BeeT_Serializer* data)
{
	BeeT_Node* node = BEET_malloc(sizeof(BeeT_Node));
	node->id = BeeT_Serializer__GetInt(data, "id");
	node->type = (NodeType)BeeT_Serializer__GetInt(data, "type");
	
	switch (node->type)
	{
	case NT_ROOT:
		node = BTN_Root_Init(node, data);
		break;
	case NT_SELECTOR:
		break;
	case NT_SEQUENCE:
		break;
	case NT_PARALLEL:
		break;
	case NT_TASK:
		break;
	}

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
		n->OnInit();
	}

	n->Update(n);

	if (n->status != NS_RUNNING)
	{
		n->OnFinish(n->status);
	}
	return n->status;
}


// BeeT Nodes Initialization by Type ---------------------------------------------

BTN_Root* BTN_Root_Init(BeeT_Node* n, const BeeT_Serializer* data)
{
	BTN_Root* btn = BEET_malloc(sizeof(BTN_Root));
	btn->node = n;
	btn->node->OnInit = &BTN_Root_OnInit;
	return btn;
}

BTN_Selector * BTN_Selector_Init(BeeT_Node * n, const BeeT_Serializer * data)
{
	BTN_Selector* btn = BEET_malloc(sizeof(BTN_Selector));
	btn->node = n;

	btn->numChilds = BeeT_Serializer__GetArraySize(data, "childs");
	int childsSize = sizeof(BeeT_Node*) * btn->numChilds;
	btn->childs = BEET_malloc(childsSize);
	for (int i = 0; i < btn->numChilds; ++i)
	{
		btn->childs[i] = BeeT_Node__Init(BeeT_Serializer__GetArray(data, "childs", i));
	}

	btn->node->OnInit = &BTN_Selector_OnInit;
	return btn;
}

BTN_Sequence * BTN_Sequence_Init(BeeT_Node * n, const BeeT_Serializer * data)
{
	BTN_Sequence* btn = BEET_malloc(sizeof(BTN_Sequence));
	btn->node = n;

	btn->numChilds = BeeT_Serializer__GetArraySize(data, "childs");
	int childsSize = sizeof(BeeT_Node*) * btn->numChilds;
	btn->childs = BEET_malloc(childsSize);
	for (int i = 0; i < btn->numChilds; ++i)
	{
		btn->childs[i] = BeeT_Node__Init(BeeT_Serializer__GetArray(data, "childs", i));
	}

	btn->node->OnInit = &BTN_Sequence_OnInit;
	return btn;
}

BTN_Task * BTN_Task_Init(BeeT_Node * n, const BeeT_Serializer * data)
{
	BTN_Task* btn = BEET_malloc(sizeof(BTN_Task));
	btn->node = n;

	btn->node->OnInit = &BTN_Task_OnInit;
	return btn;
}


// BeeT Node OnInit functions -------------------------------------------------------------

void BTN_Root_OnInit()
{}

void BTN_Selector_OnInit()
{}

void BTN_Sequence_OnInit()
{}

void BTN_Task_OnInit()
{}