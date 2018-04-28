#include "beet.h"
#include "BeeT_serializer.h"
#include "BeeT_behaviortree.h"

#include <stdio.h>
#include <stdlib.h>

//-----------------------------------------------------------------
// Helpers
//-----------------------------------------------------------------

void * LoadFile(const char * filename, int * outFileSize)
{
	BEET_ASSERT(filename != NULL);
	if (outFileSize)
		*outFileSize = 0;

	FILE* file;
	if ((file = fopen(filename, "r")) == NULL)
		return NULL;

	int fileSize;
	if (fseek(file, 0, SEEK_END) || (fileSize = (int)ftell(file)) == -1 || fseek(file, 0, SEEK_SET))
	{
		fclose(file);
		return NULL;
	}

	void* fileData = BEET_malloc(fileSize);
	if (fileData == NULL)
	{
		fclose(file);
		return NULL;
	}

	if (fread(fileData, 1, (size_t)fileSize, file) != (size_t)fileSize)
	{
		fclose(file);
		BEET_free(fileData);
		return NULL;
	}

	fclose(file);
	if (outFileSize)
		*outFileSize = fileSize;

	return fileData;
}

//-----------------------------------------------------------------
// Context
//-----------------------------------------------------------------

static BeetContext beetDefaultContext;
#define BEET_GLOBAL_CONTEXT_PTR &beetDefaultContext;
BeetContext* g_Beet = BEET_GLOBAL_CONTEXT_PTR;

void BeetContext__Init(BeetContext* ctx)
{
	ctx->initialized = BEET_FALSE;
	ctx->maxNumTreesLoaded = 32;
	ctx->numTreesLoaded = 0;
	ctx->trees = (BeeT_BehaviorTree**)BEET_malloc(ctx->maxNumTreesLoaded * sizeof(BeeT_BehaviorTree*));
}

void BeetContext__Destroy(BeetContext* ctx)
{
	for (int i = 0; i < ctx->numTreesLoaded; ++i)
		BeeT_BehaviorTree__Destroy(ctx->trees[i]);
	BEET_free(ctx->trees);
	ctx->trees = NULL;
	ctx->numTreesLoaded = 0;
	ctx->initialized = BEET_FALSE;
}

unsigned int BeetContext__AddTree(BeetContext* ctx, BeeT_BehaviorTree* bt)
{
	if (ctx->numTreesLoaded == ctx->maxNumTreesLoaded)
	{
		ctx->maxNumTreesLoaded += 32;
		*ctx->trees = (BeeT_BehaviorTree*)BEET_realloc(ctx->trees, sizeof(BeeT_BehaviorTree*) * ctx->maxNumTreesLoaded);
	}

	ctx->trees[ctx->numTreesLoaded++] = bt;
	return ctx->numTreesLoaded - 1;
}

BeeT_BehaviorTree* BeeTContext__GetTree(BeetContext* ctx, unsigned int btId)
{
	return (btId < ctx->numTreesLoaded) ? ctx->trees[btId] : NULL;
}
//-----------------------------------------------------------------
// BeeT API
//-----------------------------------------------------------------

void BEET_Init()
{
	BeetContext__Init(g_Beet);
	g_Beet->initialized = BEET_TRUE;
}

void BEET_Shutdown()
{
	if (g_Beet->initialized == BEET_FALSE)
		return;

	BeetContext__Destroy(g_Beet);
}

unsigned int BEET_LoadBehaviorTree(const char * buffer, int size)
{
	BEET_ASSERT(buffer != NULL);

	BeeT_Serializer* parser = BeeT_Serializer__CreateFromBuffer(buffer);
	BeeT_BehaviorTree* bt = BeeT_BehaviorTree__Init(parser);
	if (bt == NULL)
		return 0;
	
	unsigned int uid = BeetContext__AddTree(g_Beet, bt);

	BeeT_Serializer__Destroy(parser);

	return uid;
}

unsigned int BEET_LoadBehaviorTreeFromFile(const char * filename)
{
	BEET_ASSERT(filename != NULL);
	int fileSize = 0;
	char* fileData = (char*)LoadFile(filename, &fileSize);
	unsigned int result = 0;
	if (fileData != NULL)	// If fileData is NULL, the filename could not be found or loaded.
	{
		result = BEET_LoadBehaviorTree(fileData, fileSize);
		BEET_free(fileData);
	}
	return result;
}

BEET_API void BEET_ExecuteBehaviorTree(unsigned int id)
{
	BeeT_BehaviorTree* bt = BeeTContext__GetTree(g_Beet, id);
	if (bt)
	{
		bt->StartBehavior(bt, bt->rootNode, NULL);
		bt->Update(bt);
	}
}

void BEET_GetAllTasksNamesRecursive(BeeT_Node* n, dequeue* listNames)
{
	if (n != NULL)
	{
		switch (n->type)
		{
		case NT_ROOT:
			BEET_GetAllTasksNamesRecursive(((BTN_Root*)n)->startNode, listNames);
			break;
		case NT_SELECTOR:
		case NT_SEQUENCE:
		{
			dequeue* childs = ((BTN_Composite*)n)->childs;
			node_deq* it = childs->head;
			while (it != NULL)
			{
				BEET_GetAllTasksNamesRecursive((BeeT_Node*)it->data, listNames);
				it = it->next;
			}
		}
		break;
		case NT_PARALLEL:
			break;
		case NT_TASK:
			dequeue_push_back(listNames, ((BTN_Task*)n)->name);
			break;
		}
	}
}

int BEET_GetAllTasksNames(unsigned int btId, dequeue * listNames)
{
	BEET_ASSERT(listNames != NULL);
	int size = 0;
	BeeT_BehaviorTree* bt = BeeTContext__GetTree(g_Beet, btId);
	if(!dequeue_is_empty(listNames))
		dequeue_clear(listNames);
	if (bt != NULL)
	{
		BEET_GetAllTasksNamesRecursive(bt->rootNode, listNames);
		size = (int)dequeue_size(listNames);
	}
	return size;
}

BTN_Task* FindTaskByNameRecurisve(BeeT_Node* n, const char* name)
{
	BTN_Task* result = NULL;
	if (n != NULL)
	{
		switch (n->type)
		{
		case NT_ROOT:
			result = FindTaskByNameRecurisve(((BTN_Root*)n)->startNode, name);
			break;
		case NT_SELECTOR:
		case NT_SEQUENCE:
		{
			dequeue* childs = ((BTN_Composite*)n)->childs;
			node_deq* it = childs->head;
			while (it != NULL)
			{
				result = FindTaskByNameRecurisve((BeeT_Node*)it->data, name);
				if (result != NULL)
					break;
				it = it->next;
			}
		}
		break;
		case NT_PARALLEL:
			break;
		case NT_TASK:
			if (strcmp(((BTN_Task*)n)->name, name) == 0)
			{
				result = (BTN_Task*)n;
			}
			break;
		}
	}
	return result;
}

int BEET_SetTaskCallbackFunc(unsigned int btId, const char * task, beetCallbackFunc callback)
{
	BeeT_BehaviorTree* bt = BeeTContext__GetTree(g_Beet, btId);
	if (bt != NULL)
	{
		BTN_Task* taskNode = FindTaskByNameRecurisve(bt->rootNode, task);
		if (taskNode != NULL)
		{
			taskNode->callbackFunc = callback;
			return 1;
		}
	}
	return 0;
}

BEET_bool BEET_BBGetBool(unsigned int btId, const char * varName)
{
	BeeT_BehaviorTree* bt = BeeTContext__GetTree(g_Beet, btId);
	if (bt != NULL)
	{
		
	}
	return BEET_FALSE;
}

 int BEET_BBGetInt(unsigned int btId, const char * varName)
{
	return  int();
}

 float BEET_BBGetFloat(unsigned int btId, const char * varName)
{
	return  float();
}

 char * BEET_BBGetString(unsigned int btId, const char * varName)
{
	return NULL;
}

 BEET_bool BEET_BBSetBool(unsigned int btId, const char * varName, BEET_bool value)
{
	return  BEET_bool();
}

 BEET_bool BEET_BBSetInt(unsigned int btId, const char * varName, int value)
{
	return  BEET_bool();
}

 BEET_bool BEET_BBSetFloat(unsigned int btId, const char * varName, float value)
{
	return  BEET_bool();
}

 BEET_bool BEET_BBSetString(unsigned int btId, const char * varName, const char * value)
{
	return  BEET_bool();
}

size_t BEET_BehaviorTreeCount()
{
	return g_Beet->numTreesLoaded;
}
