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

//-----------------------------------------------------------------
// BeeT API
//-----------------------------------------------------------------

void BeeT::Init()
{
	g_Beet->initialized = true;
}

void BeeT::Shutdown()
{
	if (g_Beet->initialized == false)
		return;

	for (auto bt : g_Beet->trees)
		delete bt;

	g_Beet->initialized = false;
}

int BeeT::LoadBehaviorTree(const char * buffer, int size)
{
	BEET_ASSERT(buffer != NULL);

	BeeT_Serializer parser(buffer);
	BeeT_BehaviorTree* bt = new BeeT_BehaviorTree(parser);
	if (bt == NULL)
		return -1;
	g_Beet->trees.push_back(bt);
	return bt->uid;
}

int BeeT::LoadBehaviorTree(const char * filename)
{
	BEET_ASSERT(filename != NULL);
	int fileSize = 0;
	char* fileData = (char*)BeeT::LoadFile(filename, &fileSize);
	int result = -1;
	if (fileData != NULL)	// If fileData is NULL, the filename could not be found or loaded.
	{
		result = BeeT::LoadBehaviorTree(fileData, fileSize);
		BeeT::MemFree(fileData);
	}
	return result;
}

size_t BeeT::BehaviorTreeCount()
{
	return g_Beet->trees.size();
}

void * BeeT::MemAlloc(size_t size)
{
	return malloc(size);
}

void BeeT::MemFree(void * ptr)
{
	BEET_ASSERT(ptr);
	return free(ptr);
}

void * BeeT::LoadFile(const char * filename, int * outFileSize)
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

	void* fileData = BeeT::MemAlloc(fileSize);
	if (fileData == NULL)
	{
		fclose(file);
		return NULL;
	}

	if (fread(fileData, 1, (size_t)fileSize, file) != (size_t)fileSize)
	{
		fclose(file);
		BeeT::MemFree(fileData);
		return NULL;
	}

	fclose(file);
	if (outFileSize)
		*outFileSize = fileSize;

	return fileData;
}
