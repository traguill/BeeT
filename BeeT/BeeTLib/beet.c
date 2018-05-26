#include "beet.h"
#include "BeeT_serializer.h"
#include "BeeT_behaviortree.h"
#include "BeeT_debugger.h"

#include <stdio.h>
#include <stdlib.h>


static BeetContext beetDefaultContext;
#define BEET_GLOBAL_CONTEXT_PTR &beetDefaultContext;
BeetContext* g_Beet = BEET_GLOBAL_CONTEXT_PTR;

static BeeT_debugger beetDebugger;
#define BEET_GLOBAL_DEBUGGER_PTR &beetDebugger;
BeeT_debugger* g_Debug = BEET_GLOBAL_DEBUGGER_PTR;

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
	return ((int)btId < ctx->numTreesLoaded) ? ctx->trees[btId] : NULL;
}

//-----------------------------------------------------------------
// BeeT Debugger
//-----------------------------------------------------------------

BEET_bool BEET_InitDebugger(int port)
{
	return BeeT_Debugger_Init(g_Debug, port);
}

void BEET_DebuggerSetActive(BEET_bool active)
{
	BEET_ASSERT(g_Debug->initialized);
	g_Debug->enabled = active;
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


void BEET_Tick()
{
	for (int i = 0; i < g_Beet->numTreesLoaded; i++)
		g_Beet->trees[i]->Update(g_Beet->trees[i]);
	if (g_Debug->enabled)
		BeeT_Debugger_Tick(g_Debug);
}

unsigned int BEET_LoadBehaviorTree(const char * buffer, int size)
{
	BEET_ASSERT(buffer != NULL);

	BeeT_Serializer* parser = BeeT_Serializer__CreateFromBuffer(buffer);
	BeeT_BehaviorTree* bt = BeeT_BehaviorTree__Init(parser);
	if (bt == NULL)
		return 0;
	
	unsigned int uid = BeetContext__AddTree(g_Beet, bt);
	if (g_Debug->enabled)
		BeeT_Debugger_LoadBT(g_Debug, buffer, size, uid);
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

BBVar* BEET_QuickFindVar(unsigned int btId, const char* varName)
{
	BeeT_BehaviorTree* bt = BeeTContext__GetTree(g_Beet, btId);
	if (bt != NULL)
		return bt->bb->FindVar(bt->bb, varName);
	else
		return NULL;
}

BEET_bool BEET_BBGetBool(unsigned int btId, const char * varName)
{
	BBVar* var = BEET_QuickFindVar(btId, varName);
	return var ? *(BEET_bool*)var->data : BEET_FALSE;
}

int BEET_BBGetInt(unsigned int btId, const char * varName)
{
	BBVar* var = BEET_QuickFindVar(btId, varName);
	return var ? *(int*)var->data : 0;
}

float BEET_BBGetFloat(unsigned int btId, const char * varName)
{
	BBVar* var = BEET_QuickFindVar(btId, varName);
	return var ? *(float*)var->data : 0.0f;
}

const char * BEET_BBGetString(unsigned int btId, const char * varName)
{
	BBVar* var = BEET_QuickFindVar(btId, varName);
	return var ? (const char*)var->data : "";
}

BEET_bool BEET_BBSetBool(unsigned int btId, const char * varName, BEET_bool value)
{
	BBVar* var = BEET_QuickFindVar(btId, varName);
	if (var)
	{
		*((BEET_bool*)var->data) = value;
		return BEET_TRUE;
	}
	return BEET_FALSE;
}

 BEET_bool BEET_BBSetInt(unsigned int btId, const char * varName, int value)
{
	BBVar* var = BEET_QuickFindVar(btId, varName);
	if (var)
	{
		*((int*)var->data) = value;
		return BEET_TRUE;
	}
	return  BEET_FALSE;
}

 BEET_bool BEET_BBSetFloat(unsigned int btId, const char * varName, float value)
{
	 BBVar* var = BEET_QuickFindVar(btId, varName);
	 if (var)
	 {
		 *((float*)var->data) = value;
		 return BEET_TRUE;
	 }
	 return  BEET_FALSE;
}

 BEET_bool BEET_BBSetString(unsigned int btId, const char * varName, const char * value)
{
	 BBVar* var = BEET_QuickFindVar(btId, varName);
	 if (var)
	 {
		 BEET_free(var->data);
		 unsigned int bvLength = strlen(value) + 1;
		 var->data = BEET_malloc(bvLength);
		 strcpy(var->data, value);
		 return BEET_TRUE;
	 }
	 return  BEET_FALSE;
}

size_t BEET_BehaviorTreeCount()
{
	return g_Beet->numTreesLoaded;
}
