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
	BeeT_BehaviorTree* bt = g_Beet->trees[id];
	bt->StartBehavior(bt, bt->rootNode, NULL);
	bt->Update(bt);
}

size_t BEET_BehaviorTreeCount()
{
	return g_Beet->numTreesLoaded;
}
