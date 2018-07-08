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
	ctx->treesCapacity = 32;
	ctx->treesSize = 0;
	ctx->treesToRemove = InitDequeue();
	ctx->trees = (BeeT_BehaviorTree**)BEET_calloc(ctx->treesCapacity, sizeof(BeeT_BehaviorTree*));
}

void BeetContext__Destroy(BeetContext* ctx)
{
	for (int i = 0; i < ctx->treesCapacity; ++i)
		if(ctx->trees[i])
			BeeT_BehaviorTree__Destroy(ctx->trees[i]);
	BEET_free(ctx->trees);
	ctx->trees = NULL;
	ctx->treesSize = 0;
	ctx->initialized = BEET_FALSE;
	DestroyDequeue(ctx->treesToRemove);
}

unsigned int BeetContext__AddTree(BeetContext* ctx, BeeT_BehaviorTree* bt)
{
	if (ctx->treesSize == ctx->treesCapacity)
	{
		ctx->treesCapacity += 32;
		ctx->trees = (BeeT_BehaviorTree*)BEET_realloc(ctx->trees, sizeof(BeeT_BehaviorTree*) * ctx->treesCapacity);
	}
	int id = ctx->treesSize;
	for (int i = 0; i < ctx->treesCapacity; i++)
	{
		id += i;
		if (id >= ctx->treesCapacity)
			id = 0;
		if(ctx->trees[id] == NULL)
		{
			ctx->trees[id] = bt;
			bt->instanceUID = id;
			ctx->treesSize++;
			break;
		}
	}
	return bt->instanceUID;
}

BeeT_BehaviorTree* BeeTContext__GetTree(BeetContext* ctx, unsigned int btId)
{
	return ((int)btId < ctx->treesCapacity) ? ctx->trees[btId] : NULL;
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
	g_Beet->dt = deltaTime;

	if (dequeue_is_empty(g_Beet->treesToRemove) == BEET_FALSE)
	{
		node_deq* item = dequeue_head(g_Beet->treesToRemove);
		while (item)
		{
			BeeT_BehaviorTree* bt = BeeTContext__GetTree(g_Beet, (unsigned int)item->data);
			if (bt)
			{
				BeeT_BehaviorTree__Destroy(bt);
				g_Beet->trees[(unsigned int)item->data] = NULL;
			}
			item = item->next;
		}
		dequeue_clear(g_Beet->treesToRemove);
	}
	
	for (int i = 0; i < g_Beet->treesCapacity; i++)
		if(g_Beet->trees[i] && g_Beet->trees[i]->paused == BEET_FALSE)
			g_Beet->trees[i]->Update(g_Beet->trees[i]);
	
	if(g_Debug->initialized)
		BeeT_Debugger_Tick(g_Debug);
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

BEET_API int BEET_PauseBehaviorTree(unsigned int btId)
{
	BeeT_BehaviorTree* foundBT = BeeTContext__GetTree(g_Beet, btId);
	if (foundBT == NULL)
		return BEET_FALSE;
	foundBT->paused = BEET_TRUE;
	return BEET_TRUE;
}

BEET_API int BEET_ResumeBehaviorTree(unsigned int btId)
{
	BeeT_BehaviorTree* foundBT = BeeTContext__GetTree(g_Beet, btId);
	if (foundBT == NULL)
		return BEET_FALSE;
	foundBT->paused = BEET_FALSE;
	return BEET_TRUE;
}

BEET_API int BEET_CloseBehaviorTree(unsigned int btId)
{
	BeeT_BehaviorTree* bt = BeeTContext__GetTree(g_Beet, btId);
	if (bt != NULL)
	{
		bt->paused = BEET_TRUE;
		dequeue_push_back(g_Beet->treesToRemove, btId);
		return BEET_TRUE;
	}
	else
		return BEET_FALSE;
}

BEET_API BEET_bool BEET_BehaviorTreeIsPaused(unsigned int btId)
{
	BeeT_BehaviorTree* foundBT = BeeTContext__GetTree(g_Beet, btId);
	if (foundBT == NULL)
		return BEET_FALSE;
	return foundBT->paused;
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

float2 BEET_BBGetVector2(unsigned int btId, const char * varName)
{
	BBVar* var = BEET_QuickFindVar(btId, varName, NULL);
	float2 ret;
	if (var)
		ret = *(float2*)var->data;
	else
		ret.x = ret.y = 0.0f;
	return ret;
}

BEET_bool BEET_BBSetVector2(unsigned int btId, const char * varName, const float2 * value)
{
	BeeT_BehaviorTree* bt = NULL;
	BBVar* var = BEET_QuickFindVar(btId, varName, &bt);
	if (var)
	{

		if (bt->debug != NULL && !Float2Equal((float2*)var->data, value))
			BeeT_dBT_bbVector2(bt->debug, var, value);
		*(float2*)var->data = *value;
		return BEET_TRUE;
	}
	return  BEET_FALSE;
}

BEET_bool BEET_BBSetVector2Param(unsigned int btId, const char * varName, float x, float y)
{
	BeeT_BehaviorTree* bt = NULL;
	BBVar* var = BEET_QuickFindVar(btId, varName, &bt);
	if (var)
	{
		float2 value;
		value.x = x;
		value.y = y;
		if (bt->debug != NULL && !Float2Equal((float2*)var->data, &value))
			BeeT_dBT_bbVector2(bt->debug, var, &value);
		*(float2*)var->data = value;
		return BEET_TRUE;
	}
	return  BEET_FALSE;
}

float3 BEET_BBGetVector3(unsigned int btId, const char * varName)
{
	BBVar* var = BEET_QuickFindVar(btId, varName, NULL);
	float3 ret;
	if (var)
		ret = *(float3*)var->data;
	else
		ret.x = ret.y = ret.z = 0.0f;
	return ret;
}

BEET_bool BEET_BBSetVector3(unsigned int btId, const char * varName, const float3 * value)
{
	BeeT_BehaviorTree* bt = NULL;
	BBVar* var = BEET_QuickFindVar(btId, varName, &bt);
	if (var)
	{

		if (bt->debug != NULL && !Float3Equal((float3*)var->data, value))
			BeeT_dBT_bbVector3(bt->debug, var, value);
		*(float3*)var->data = *value;
		return BEET_TRUE;
	}
	return  BEET_FALSE;
}

BEET_bool BEET_BBSetVector3Param(unsigned int btId, const char * varName, float x, float y, float z)
{
	BeeT_BehaviorTree* bt = NULL;
	BBVar* var = BEET_QuickFindVar(btId, varName, &bt);
	if (var)
	{
		float3 value;
		value.x = x;
		value.y = y;
		value.z = z;
		if (bt->debug != NULL && !Float2Equal((float3*)var->data, &value))
			BeeT_dBT_bbVector3(bt->debug, var, &value);
		*(float3*)var->data = value;
		return BEET_TRUE;
	}
	return  BEET_FALSE;
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
	return g_Beet->treesSize;
}

BeetContext * BeeT_GetContext()
{
	return g_Beet;
}
