#include "BeeT_node.h"
#include "BeeT_behaviortree.h"
#include "BeeT_DBG_behaviortree.h"
#include <stdio.h>
#include "beet.h"

BeeT_Node* BeeT_Node__Init(const BeeT_Serializer* data, BeeT_BehaviorTree* bt)
{
	
	BeeT_Node* node = NULL;
	NodeType type = (NodeType)BeeT_Serializer_GetInt(data, "type");
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
	case NT_WAIT:
		node = (BeeT_Node*)BTN_Wait_Init(data);
		break;
	}
	node->id = BeeT_Serializer_GetInt(data, "id");
	node->type = (NodeType)BeeT_Serializer_GetInt(data, "type");
	node->bt = bt;
	node->observer = NULL;
	node->observerNode = NULL;
	node->status = NS_INVALID;
	node->Tick = &Tick;

	node->decorators = InitDequeue();
	int decSize = BeeT_Serializer_GetArraySize(data, "decorators");
	for (int i = 0; i < decSize; ++i)
	{
		BeeT_Serializer* decData = BeeT_Serializer_GetArray(data, "decorators", i);
		BeeT_decorator* dec = BeeT_Decorator_Init(decData, bt->bb);
		dequeue_push_back(node->decorators, dec);
		BEET_free(decData);
	}

	return node;
}

void BeeT_Node__Destroy(BeeT_Node * self)
{
	switch (self->type)
	{
	case NT_ROOT:
		BTN_Root_OnDestroy((BTN_Root*)self);
		break;
	case NT_SELECTOR:
		BTN_Composite_OnDestroy((BTN_Composite*)self);
		break;
	case NT_SEQUENCE:
		BTN_Composite_OnDestroy((BTN_Composite*)self);
		break;
	case NT_PARALLEL:
		break;
	case NT_TASK:
		BTN_Task_OnDestroy((BTN_Task*)self);
		break;
	case NT_WAIT:
		BTN_Wait_OnDestroy((BTN_Wait*)self);
		break;
	}
	if (self->decorators)
	{
		node_deq* item = dequeue_head(self->decorators);
		while (item)
		{
			BeeT_Decorator_Destroy((BeeT_decorator*)item->data);
			item = item->next;
		}
		DestroyDequeue(self->decorators);
	}
	BEET_free(self);
}

void BeeT_Node_ChangeStatus(BeeT_Node * node, NodeStatus newStatus)
{
	if (node->status == newStatus)
		return;

	if (node->bt->debug && !(node->status == NS_RUNNING && newStatus == NS_SUSPENDED) && !(node->status == NS_SUSPENDED && newStatus == NS_RUNNING)) // Consider RUNNING = SUSPENDED. Don't send as change status.
		BeeT_dBT_NodeReturnStatus(node->bt->debug, node, newStatus);
	node->status = newStatus;
}

NodeStatus Tick(BeeT_Node* n)
{
	if (n->status == NS_INVALID)
	{
		//Check if all decorators return true
		if (!dequeue_is_empty(n->decorators))
		{
			node_deq* item = n->decorators->head;
			while (item)
			{
				BeeT_decorator* dec = (BeeT_decorator*)item->data;
				if (dec->Pass(dec) == BEET_FALSE)
				{
					BeeT_Node_ChangeStatus(n, NS_FAILURE);		// Abort execution because a decorator failed
					return n->status;
				}
				item = item->next;
			}
		}

		n->OnInit(n);
		BeeT_Node_ChangeStatus(n, NS_RUNNING);
	}

	if (n->status != NS_SUCCESS && n->status != NS_FAILURE)
	{
		NodeStatus status = n->Update(n);
		BeeT_Node_ChangeStatus(n, status);
	}
	if (n->status != NS_RUNNING && n->status != NS_SUSPENDED)
	{
		n->OnFinish(n, n->status);
	}
	return n->status;
}


// BeeT Nodes Initialization by Type ---------------------------------------------

BTN_Root* BTN_Root_Init(const BeeT_Serializer* data, BeeT_BehaviorTree* bt)
{
	BTN_Root* btn = (BTN_Root*)BEET_malloc(sizeof(BTN_Root));

	int size = BeeT_Serializer_GetArraySize(data, "childs");
	if (BeeT_Serializer_GetArraySize(data, "childs") != 0)
	{
		btn->startNode = BeeT_Node__Init(BeeT_Serializer_GetArray(data, "childs", 0), bt);
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
	unsigned int numChilds = BeeT_Serializer_GetArraySize(data, "childs");
	for (unsigned int i = 0; i < numChilds; ++i)
	{
		BeeT_Node* child = BeeT_Node__Init(BeeT_Serializer_GetArray(data, "childs", i), bt);
		dequeue_push_back(btn->childs, child);
	}

	btn->node.OnInit = &BTN_Selector_OnInit;
	btn->node.Update = &BTN_Selector_Update;
	btn->node.OnFinish = &BTN_Selector_OnFinish;
	return btn;
}

BTN_Sequence * BTN_Sequence_Init(const BeeT_Serializer * data, BeeT_BehaviorTree* bt)
{
	BTN_Sequence* btn = (BTN_Sequence*)BEET_malloc(sizeof(BTN_Sequence));

	btn->childs = InitDequeue();
	unsigned int numChilds = BeeT_Serializer_GetArraySize(data, "childs");
	for (unsigned int i = 0; i < numChilds; ++i)
	{
		BeeT_Node* child = BeeT_Node__Init(BeeT_Serializer_GetArray(data, "childs", i), bt);
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

	const char* name = BeeT_Serializer_GetString(data, "name");
	unsigned int length = strlen(name) + 1;
	btn->name = (char*)BEET_malloc(length);
	strcpy(btn->name, name);

	btn->node.OnInit = &BTN_Task_OnInit;
	btn->node.Update = &BTN_Task_Update;
	btn->node.OnFinish = &BTN_Task_OnFinish;
	return btn;
}

BTN_Wait * BTN_Wait_Init(const BeeT_Serializer * data)
{
	BTN_Wait* btn = (BTN_Wait*)BEET_malloc(sizeof(BTN_Wait));

	btn->seconds = BeeT_Serializer_GetFloat(data, "extraData");
	btn->timer = 0.0f;

	btn->node.OnInit = &BTN_Wait_OnInit;
	btn->node.Update = &BTN_Wait_Update;
	btn->node.OnFinish = &BTN_Wait_OnFinish;
	return btn;
}

// BeeT Node OnInit functions -------------------------------------------------------------

void BTN_Root_OnInit(BeeT_Node* self)
{
	BTN_Root* btn = (BTN_Root*)self;
	if (btn->startNode)
	{
		btn->startNode->status = NS_INVALID;
		btn->startNode->observerNode = self;
		btn->node.bt->StartBehavior(btn->node.bt, btn->startNode, BTN_Root_TreeFinish);
	}
}

void BTN_Selector_OnInit(BeeT_Node* self)
{
	BTN_Selector* btn = (BTN_Selector*)self;
	node_deq* item = dequeue_head(btn->childs);
	while (item)
	{
		BeeT_Node* n = (BeeT_Node*)item->data;
		n->status = NS_INVALID;
		item = item->next;
	}
	btn->current = dequeue_head(btn->childs);
	BeeT_Node* current_node = (BeeT_Node*)dequeue_front(btn->childs);
	current_node->observerNode = self;
	btn->node.bt->StartBehavior(btn->node.bt, current_node, BTN_Selector_OnChildFinish);
}

void BTN_Sequence_OnInit(BeeT_Node* self)
{
	BTN_Sequence* btn = (BTN_Sequence*)self;
	node_deq* item = dequeue_head(btn->childs);
	while (item)
	{
		BeeT_Node* n = (BeeT_Node*)item->data;
		n->status = NS_INVALID;
		item = item->next;
	}
	btn->current = dequeue_head(btn->childs);
	BeeT_Node* current_node = (BeeT_Node*)dequeue_front(btn->childs);
	current_node->observerNode = self;
	btn->node.bt->StartBehavior(btn->node.bt, current_node, BTN_Sequence_OnChildFinish);
}

void BTN_Task_OnInit(BeeT_Node* self)
{}

void BTN_Wait_OnInit(BeeT_Node * self)
{
	BTN_Wait* btn = (BTN_Wait*)self;
	btn->timer = 0.0f;
}


// BeeT Node Update functions --------------------------------------------------------------

NodeStatus BTN_Root_Update(BeeT_Node* self)
{
	return NS_SUSPENDED;
}

NodeStatus BTN_Selector_Update(BeeT_Node* self)
{
	return NS_SUSPENDED;
}

NodeStatus BTN_Sequence_Update(BeeT_Node* self)
{
	return NS_SUSPENDED;
}

NodeStatus BTN_Task_Update(BeeT_Node* self)
{
	BTN_Task* btn = (BTN_Task*)self;
	BeetContext* context = BeeT_GetContext();
	BEET_ASSERT(context != NULL);
	return context->taskCallbackFunc(btn->node.bt->uid, btn->name);
}

NodeStatus BTN_Wait_Update(BeeT_Node * self)
{
	BTN_Wait* btn = (BTN_Wait*)self;
	btn->timer += BeeT_GetContext()->dt;
	if (btn->timer >= btn->seconds)
		return NS_SUCCESS;
	return NS_RUNNING;
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

void BTN_Wait_OnFinish(BeeT_Node * self, NodeStatus status)
{}

// BeeT Node OnDestroy functions ------------------------------------------------------------

void BTN_Root_OnDestroy(BTN_Root * self)
{
	if (self->startNode)
	{
		BeeT_Node__Destroy(self->startNode);
	}
}

void BTN_Composite_OnDestroy(BTN_Composite * self)
{
	if (self->childs)
	{
		do
		{
			BeeT_Node* n = dequeue_front(self->childs);
			dequeue_pop_front(self->childs);
			if (n)
			{
				BeeT_Node__Destroy(n);
			}
		} while (!dequeue_is_empty(self->childs));
	}
}

void BTN_Task_OnDestroy(BTN_Task * self)
{
	if (self->name)
	{
		BEET_free(self->name);
	}
}

void BTN_Wait_OnDestroy(BTN_Wait * self)
{
}

// BeeT Node Observer functions -------------------------------------------------------------

void BTN_Root_TreeFinish(BeeT_Node * self, NodeStatus s)
{
	self->bt->StopBehavior(self, s);
	self->status = NS_INVALID;
	dequeue_clear(self->bt->runningNodes);	// Clear running nodes
	dequeue_push_back(self->bt->runningNodes, NULL); // Push a NULL node to end the Tick
	dequeue_push_back(self->bt->runningNodes, self); // Push Root node to start the next Tick
	if (self->bt->debug)
		BeeT_dBT_BTEnd(self->bt->debug);
}

void BTN_Selector_OnChildFinish(BeeT_Node* self, NodeStatus s)
{
	BTN_Selector* btn = (BTN_Selector*)self;
	BeeT_Node* child = (BeeT_Node*)btn->current->data;
	if (child->status == NS_SUCCESS)
	{
		btn->node.bt->StopBehavior(self, NS_FAILURE);
		return;
	}

	BEET_ASSERT(child->status == NS_FAILURE);
	btn->current = btn->current->next;
	if (btn->current == NULL)
	{
		btn->node.bt->StopBehavior(self, NS_SUCCESS);
	}
	else
	{
		BeeT_Node* n_current = (BeeT_Node*)btn->current->data;
		n_current->observerNode = self;
		btn->node.bt->StartBehavior(btn->node.bt, n_current, &BTN_Selector_OnChildFinish);
	}
}

void BTN_Sequence_OnChildFinish(BeeT_Node* self, NodeStatus s)
{
	BTN_Sequence* btn = (BTN_Sequence*)self;
	BeeT_Node* child = (BeeT_Node*)btn->current->data;
	if (child->status == NS_FAILURE)
	{
		btn->node.bt->StopBehavior(self, NS_FAILURE);
		return;
	}

	BEET_ASSERT(child->status == NS_SUCCESS);
	btn->current = btn->current->next;
	if (btn->current == NULL)
	{
		btn->node.bt->StopBehavior(self, NS_SUCCESS);
	}
	else
	{
		BeeT_Node* n_current = (BeeT_Node*)btn->current->data;
		n_current->observerNode = self;
		btn->node.bt->StartBehavior(btn->node.bt, n_current, &BTN_Sequence_OnChildFinish);
	}
}