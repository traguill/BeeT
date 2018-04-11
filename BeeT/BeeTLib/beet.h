#ifndef __BEET_H__
#define __BEET_H__

#include "beet_std.h"

#include "beet_begin.h"

#include "BeeT_node.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------------
typedef NodeStatus(*beetCallbackFunc)(unsigned int btId, const char* taskId);

typedef struct
{
	BEET_bool initialized;
	struct BeeT_BehaviorTree** trees;
	int numTreesLoaded;
	int maxNumTreesLoaded;
	beetCallbackFunc callbackFunc;
}BeetContext;

//--------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------

BEET_API void	BEET_Init(beetCallbackFunc callback);
BEET_API void	BEET_Shutdown();

//--------------------------------------------------------------------------------
// BehaviorTree
//--------------------------------------------------------------------------------

BEET_API unsigned int BEET_LoadBehaviorTree(const char* buffer, int size);

/*
*	Loads a BehaviorTree from a file path.
*	\param filename The file path
*	\return The uid of the Behavior Tree on SUCCESS, -1 on FAIL.
*/
BEET_API unsigned int BEET_LoadBehaviorTreeFromFile(const char* filename);

BEET_API void BEET_ExecuteBehaviorTree(unsigned int id);

//--------------------------------------------------------------------------------
// Utils
//--------------------------------------------------------------------------------

// Get the number of Behavior Trees loaded in memory.
BEET_API size_t	BEET_BehaviorTreeCount();

//--------------------------------------------------------------------------------
// Memory
//--------------------------------------------------------------------------------

// Call this before anything else.
// Returns 1 on sucess and 0 on failure.
BEET_API int BEET_SetMemoryFunctions(BeeT_Malloc_Function malloc_func, BeeT_Free_Function free_func, BeeT_Realloc_Function realloc_func);

#ifdef __cplusplus
}
#endif
#include "beet_end.h"
#endif // BEET_H