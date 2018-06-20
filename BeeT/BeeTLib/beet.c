#include "beet.h"
#include "BeeT_serializer.h"
#include "BeeT_behaviortree.h"
#include "BeeT_debugger.h"
#include "BeeT_DBG_behaviortree.h"

#include <stdio.h>
#include <stdlib.h>


static BeeT_debugger beetDebugger;
#define BEET_GLOBAL_DEBUGGER_PTR &beetDebugger;
BeeT_debugger* g_Debug = BEET_GLOBAL_DEBUGGER_PTR;

static BeetContext beetDefaultContext;
#define BEET_GLOBAL_CONTEXT_PTR &beetDefaultContext;
BeetContext* g_Beet = BEET_GLOBAL_CONTEXT_PTR;

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
		ctx->trees = (BeeT_BehaviorTree*)BEET_realloc(ctx->trees, sizeof(BeeT_BehaviorTree*) * ctx->maxNumTreesLoaded);
	}

	ctx->trees[ctx->numTreesLoaded++] = bt;
	bt->instanceUID = ctx->numTreesLoaded - 1;
	return bt->instanceUID;
}

BeeT_BehaviorTree* BeeTContext__GetTree(BeetContext* ctx, unsigned int btId)
{
	return ((int)btId < ctx->numTreesLoaded) ? ctx->trees[btId] : NULL;
}

//-----------------------------------------------------------------
// BeeT Debugger
//-----------------------------------------------------------------

BEET_bool BEET_InitDebugger(const char* ip, int port)
{
	return BeeT_Debugger_Init(g_Debug, ip, port);
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
	if (g_Debug->initialized)
		BeeT_Debugger_Cleanup(g_Debug);
	BeetContext__Destroy(g_Beet);
}


void BEET_Tick(float deltaTime)
{
	for (int i = 0; i < g_Beet->numTreesLoaded; i++)
		g_Beet->trees[i]->Update(g_Beet->trees[i]);
	if(g_Debug->initialized)
		BeeT_Debugger_Tick(g_Debug);
	g_Beet->dt = deltaTime;
}

unsigned int BEET_LoadBehaviorTree(const char * buffer, int size, BEET_bool debug)
{
	BEET_ASSERT(buffer != NULL);

	BeeT_Serializer* parser = BeeT_Serializer_CreateFromBuffer(buffer);
	BeeT_BehaviorTree* bt = BeeT_BehaviorTree__Init(parser);
	if (bt == NULL)
		return 0;
	
	unsigned int uid = BeetContext__AddTree(g_Beet, bt);
	if(debug)
		bt->debug = BeeT_Debugger_LoadBT(g_Debug, buffer, size);
	
	BeeT_Serializer_Destroy(parser);
	return uid;
}

unsigned int BEET_LoadBehaviorTreeFromFile(const char * filename, BEET_bool debug)
{
	BEET_ASSERT(filename != NULL);
	int fileSize = 0;
	char* fileData = (char*)LoadFile(filename, &fileSize);
	unsigned int result = 0;
	if (fileData != NULL)	// If fileData is NULL, the filename could not be found or loaded.
	{
		result = BEET_LoadBehaviorTree(fileData, fileSize, debug);
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

int BEET_SetTaskCallbackFunc(beetCallbackFunc callback)
{
	if (g_Beet == NULL)
		return (int)BEET_FALSE;
	g_Beet->taskCallbackFunc = callback;
	return (int)BEET_TRUE;
}

BBVar* BEET_QuickFindVar(unsigned int btId, const char* varName, BeeT_BehaviorTree** bt)
{
	BeeT_BehaviorTree* foundBT = BeeTContext__GetTree(g_Beet, btId);
	if(bt != NULL)
		*bt = foundBT;
	if (foundBT != NULL)
		return foundBT->bb->FindVar(foundBT->bb, varName);
	else
		return NULL;
}

BEET_bool BEET_BBGetBool(unsigned int btId, const char * varName)
{
	BBVar* var = BEET_QuickFindVar(btId, varName, NULL);
	return var ? *(BEET_bool*)var->data : BEET_FALSE;
}

int BEET_BBGetInt(unsigned int btId, const char * varName)
{
	BBVar* var = BEET_QuickFindVar(btId, varName, NULL);
	return var ? *(int*)var->data : 0;
}

float BEET_BBGetFloat(unsigned int btId, const char * varName)
{
	BBVar* var = BEET_QuickFindVar(btId, varName, NULL);
	return var ? *(float*)var->data : 0.0f;
}

const char * BEET_BBGetString(unsigned int btId, const char * varName)
{
	BBVar* var = BEET_QuickFindVar(btId, varName, NULL);
	return var ? (const char*)var->data : "";
}

BEET_bool BEET_BBSetBool(unsigned int btId, const char * varName, BEET_bool value)
{
	BeeT_BehaviorTree* bt = NULL;
	BBVar* var = BEET_QuickFindVar(btId, varName, &bt);
	if (var)
	{
		if (bt->debug != NULL && *((BEET_bool*)var->data) != value)
			BeeT_dBT_bbBool(bt->debug, var, value);
		*((BEET_bool*)var->data) = value;
		return BEET_TRUE;
	}
	return BEET_FALSE;
}

 BEET_bool BEET_BBSetInt(unsigned int btId, const char * varName, int value)
{
	BeeT_BehaviorTree* bt = NULL;
	BBVar* var = BEET_QuickFindVar(btId, varName, &bt);
	if (var)
	{
		if (bt->debug != NULL && *((int*)var->data) != value)
			BeeT_dBT_bbInt(bt->debug, var, value);
		*((int*)var->data) = value;
		return BEET_TRUE;
	}
	return  BEET_FALSE;
}

 BEET_bool BEET_BBSetFloat(unsigned int btId, const char * varName, float value)
{
	 BeeT_BehaviorTree* bt = NULL;
	 BBVar* var = BEET_QuickFindVar(btId, varName, &bt);
	 if (var)
	 {
		 if (bt->debug != NULL && *((float*)var->data) != value)
			 BeeT_dBT_bbFloat(bt->debug, var, value);
		 *((float*)var->data) = value;
		 return BEET_TRUE;
	 }
	 return  BEET_FALSE;
}

 BEET_bool BEET_BBSetString(unsigned int btId, const char * varName, const char * value)
{
	 BeeT_BehaviorTree* bt = NULL;
	 BBVar* var = BEET_QuickFindVar(btId, varName, &bt);
	 if (var)
	 {
		 if (bt->debug != NULL && strcmp((char*)var->data, value) != 0)
			 BeeT_dBT_bbString(bt->debug, var, value);
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

BeetContext * BeeT_GetContext()
{
	return g_Beet;
}
