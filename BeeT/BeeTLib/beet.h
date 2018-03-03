#ifndef __BEET_H__
#define __BEET_H__

#ifdef BEET_EXPORTS
	#define BEET_API __declspec(dllexport)
#else
	#define BEET_API __declspec(dllimport)
#endif

#ifndef BEET_ASSERT
#include <assert.h>
#define BEET_ASSERT(_EXPR)	assert(_EXPR)
#endif

struct BeetContext
{
	bool initialized = false;
};

namespace BeeT
{
	// Main
	BEET_API void	Init();
	BEET_API void	Shutdown();

	// Behavior Tree
	BEET_API bool	LoadBehaviorTree(const char* buffer, int size);
	BEET_API bool	LoadBehaviorTree(const char* path);

	//--------------------------------------------------------------------------------
	// Helpers
	//--------------------------------------------------------------------------------

	BEET_API void*	MemAlloc(size_t size);
	BEET_API void	MemFree(void* ptr);

	BEET_API void*	LoadFile(const char* filename, int* outFileSize = NULL);
}

#endif