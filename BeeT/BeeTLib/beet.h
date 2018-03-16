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


// Forward declarations

struct BeeT_BehaviorTree;

// -----------------------------------------------------------------------------------
#define MAX_NUMBER_OF_BEHAVIOR_TREES 20
struct BeetContext
{
	bool initialized = false;
	BeeT_BehaviorTree* trees[MAX_NUMBER_OF_BEHAVIOR_TREES] = {NULL};
	int numTreesLoaded = 0;
};

namespace BeeT
{
	//--------------------------------------------------------------------------------
	// Main
	//--------------------------------------------------------------------------------

	BEET_API void	Init();
	BEET_API void	Shutdown();

	//--------------------------------------------------------------------------------
	// BehaviorTree
	//--------------------------------------------------------------------------------

	BEET_API int	LoadBehaviorTree(const char* buffer, int size);

	/*
	*	Loads a BehaviorTree from a file path.
	*	\param filename The file path
	*	\return The uid of the Behavior Tree on SUCCESS, -1 on FAIL.
	*/
	BEET_API int	LoadBehaviorTree(const char* filename);

	//--------------------------------------------------------------------------------
	// Utils
	//--------------------------------------------------------------------------------

	// Get the number of Behavior Trees loaded in memory.
	BEET_API size_t	BehaviorTreeCount();

	//--------------------------------------------------------------------------------
	// Helpers
	//--------------------------------------------------------------------------------

	BEET_API void*	MemAlloc(size_t size);
	BEET_API void	MemFree(void* ptr);

	BEET_API void*	LoadFile(const char* filename, int* outFileSize = NULL);
}
#endif