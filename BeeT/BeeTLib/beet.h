#ifndef __BEET_H__
#define __BEET_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BEET_EXPORTS
	#define BEET_API __declspec(dllexport)
#else
	#define BEET_API __declspec(dllimport)
#endif

#include "BeeT_internal.h"

// -----------------------------------------------------------------------------------
typedef struct
{
	BEET_bool initialized;
	struct BeeT_BehaviorTree** trees;
	int numTreesLoaded;
	int maxNumTreesLoaded;
}BeetContext;

//--------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------

BEET_API void	BEET_Init();
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

//--------------------------------------------------------------------------------
// Utils
//--------------------------------------------------------------------------------

// Get the number of Behavior Trees loaded in memory.
BEET_API size_t	BEET_BehaviorTreeCount();

#ifdef __cplusplus
}
#endif

#endif // BEET_H