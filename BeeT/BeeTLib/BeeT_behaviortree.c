#include "BeeT_behaviortree.h"
#include "BeeT_DBG_behaviortree.h"

// TO_DEBUG
#include "DebugLibrary.h"

// Forward delcarations
void StartBehavior(BeeT_BehaviorTree*, BeeT_Node*, ObserverFunc);
void StopBehavior(BeeT_Node*, NodeStatus);
void Update(BeeT_BehaviorTree*);
BEET_bool Step(BeeT_BehaviorTree*);

BeeT_BehaviorTree* BeeT_BehaviorTree__Init(const BeeT_Serializer * data)
{
	BeeT_BehaviorTree* tree = (BeeT_BehaviorTree*)BEET_malloc(sizeof(BeeT_BehaviorTree));
	tree->bb = BeeT_Blackboard_Init(data);

	tree->uid = BeeT_Serializer_GetUInt(data, "uid");
	int rootId = BeeT_Serializer_GetInt(data, "rootNodeId");
	int numNodes = BeeT_Serializer_GetArraySize(data, "nodes");
	for (int i = 0; i < numNodes; ++i)
	{
		BeeT_Serializer* nodeData = BeeT_Serializer_GetArray(data, "nodes", i);
		if (BeeT_Serializer_GetInt(nodeData, "id") == rootId)
		{
			BeeT_Serializer* rootNodeData = BeeT_Serializer_GetArray(data, "nodes", i);
			tree->rootNode = BeeT_Node__Init(rootNodeData, tree);
			BEET_free(nodeData);
			BEET_free(rootNodeData);
			break;
		}
		BEET_free(nodeData);
	}

	tree->runningNodes = InitDequeue();
	tree->StartBehavior = &StartBehavior;
	tree->StopBehavior = &StopBehavior;
	tree->Update = &Update;
	tree->Step = &Step;
	tree->debug = NULL;

	if(tree->rootNode)
		tree->StartBehavior(tree, tree->rootNode, NULL);
	return tree;
}

void BeeT_BehaviorTree__Destroy(BeeT_BehaviorTree * self)
{
	if (self->bb)
		BeeT_Blackboard_Destroy(self->bb);
	if (self->rootNode)
	{
		BeeT_Node__Destroy(self->rootNode);
	}
	BEET_free(self);
}


// Functions -----------------------------------------------------------------------------
void StartBehavior(BeeT_BehaviorTree* bt, BeeT_Node* behavior, ObserverFunc obsFunc)
{
	if (obsFunc != NULL)
		behavior->observer = obsFunc;
	if (bt->debug)
		BeeT_dBT_NewCurrentNode(bt->debug, behavior);
	dequeue_push_front(bt->runningNodes, behavior);
}
void StopBehavior(BeeT_Node* behavior, NodeStatus resultStatus)
{
	BEET_ASSERT(resultStatus != NS_RUNNING);
	BeeT_Node_ChangeStatus(behavior, resultStatus);
	if (behavior->observer)
	{
		behavior->observer(behavior->observerNode, resultStatus);
	}
}
void Update(BeeT_BehaviorTree* bt)
{
	dequeue_push_back(bt->runningNodes, NULL); // Marks end of update
	TO_DEBUG_PrintRunningNodes(bt->runningNodes, 0);
	while (bt->Step(bt))
	{
		continue;
	}
	TO_DEBUG_PrintRunningNodes(bt->runningNodes, 1);
}
BEET_bool Step(BeeT_BehaviorTree* bt)
{
	BeeT_Node* current = (BeeT_Node*)dequeue_front(bt->runningNodes);
	dequeue_pop_front(bt->runningNodes);

	if (current == NULL)
	{
		return BEET_FALSE;
	}

	NodeStatus tickRet = current->Tick(current);
	BeeT_Node_ChangeStatus(current, tickRet);

	if (current->status != NS_RUNNING && current->status != NS_SUSPENDED)
	{
		if(current->observer)
			current->observer(current->observerNode, current->status);
		current->status = NS_INVALID; // Reset the node
	}
	else
	{
		if(current->status == NS_RUNNING)
			dequeue_push_back(bt->runningNodes, current);
	}

	return BEET_TRUE;
}