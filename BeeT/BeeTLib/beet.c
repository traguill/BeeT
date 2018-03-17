#include "beet.h"
#include "BeeT_serializer.h"
#include "BeeT_behaviortree.h"

#include <stdio.h>
#include <stdlib.h>

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
	ctx->trees = (BeeT_BehaviorTree*)malloc(ctx->maxNumTreesLoaded * sizeof(BeeT_BehaviorTree*));
}

void BeetContext__Destroy(BeetContext* ctx)
{
	for (int i = 0; i < ctx->numTreesLoaded; ++i)
		BeeT_BehaviorTree__Destroy(ctx->trees[i]);
	free(ctx->trees);
	ctx->trees = NULL;
	ctx->numTreesLoaded = 0;
	ctx->initialized = BEET_FALSE;
}

void BeetContext__AddTree(BeetContext* ctx, BeeT_BehaviorTree* bt)
{
	if (ctx->numTreesLoaded == ctx->maxNumTreesLoaded)
	{
		ctx->maxNumTreesLoaded += 32;
		ctx->trees = (BeeT_BehaviorTree*)realloc(ctx->trees, sizeof(BeeT_BehaviorTree*) * ctx->maxNumTreesLoaded);
	}

	ctx->trees[ctx->numTreesLoaded++] = bt;
}
//-----------------------------------------------------------------
// BeeT API
//-----------------------------------------------------------------

void Init()
{
	BeetContext__Init(g_Beet);
	g_Beet->initialized = BEET_TRUE;
}

void Shutdown()
{
	if (g_Beet->initialized == BEET_FALSE)
		return;

	BeetContext__Destroy(g_Beet);
}

int LoadBehaviorTree(const char * buffer, int size)
{
	BEET_ASSERT(buffer != NULL);

	BeeT_Serializer* parser = BeeT_Serializer__CreateFromBuffer(buffer);
	BeeT_BehaviorTree* bt = BeeT_BehaviorTree__Init(parser);
	if (bt == NULL)
		return -1;
	
	BeetContext__AddTree(g_Beet, bt);

	BeeT_Serializer__Destroy(parser);

	return bt->uid;
}

int LoadBehaviorTreeFromFile(const char * filename)
{
	BEET_ASSERT(filename != NULL);
	int fileSize = 0;
	char* fileData = (char*)LoadFile(filename, &fileSize);
	int result = -1;
	if (fileData != NULL)	// If fileData is NULL, the filename could not be found or loaded.
	{
		result = LoadBehaviorTree(fileData, fileSize);
		MemFree(fileData);
	}
	return result;
}

size_t BehaviorTreeCount()
{
	return g_Beet->numTreesLoaded;
}

void * MemAlloc(size_t size)
{
	return malloc(size);
}

void MemFree(void * ptr)
{
	BEET_ASSERT(ptr);
	free(ptr);
}

void * LoadFile(const char * filename, int * outFileSize)
{
	BEET_ASSERT(filename != NULL);
	if (outFileSize)
		*outFileSize = 0;

	FILE* file ;
	if ((file = fopen(filename, "r")) == NULL)
		return NULL;

	int fileSize;
	if (fseek(file, 0, SEEK_END) || (fileSize = (int)ftell(file)) == -1 || fseek(file, 0, SEEK_SET))
	{
		fclose(file);
		return NULL;
	}

	void* fileData = MemAlloc(fileSize);
	if (fileData == NULL)
	{
		fclose(file);
		return NULL;
	}

	if (fread(fileData, 1, (size_t)fileSize, file) != (size_t)fileSize)
	{
		fclose(file);
		MemFree(fileData);
		return NULL;
	}

	fclose(file);
	if (outFileSize)
		*outFileSize = fileSize;

	return fileData;
}
