#ifndef __BEET_H__
#define __BEET_H__

#include "beet_std.h"

#include "beet_begin.h"

#include "BeeT_node.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------------

typedef struct
{
	BEET_bool initialized;
	struct BeeT_BehaviorTree** trees;
	int numTreesLoaded;
	int maxNumTreesLoaded;
	float dt;
	beetCallbackFunc taskCallbackFunc;
}BeetContext;

//--------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------

BEET_API void		BEET_Init();
BEET_API void		BEET_Shutdown();

BEET_API BEET_bool	BEET_InitDebugger(int port);

BEET_API void		BEET_Tick(float deltaTime);

//--------------------------------------------------------------------------------
// BehaviorTree
//--------------------------------------------------------------------------------

BEET_API unsigned int BEET_LoadBehaviorTree(const char* buffer, int size, BEET_bool debug);

/*
*	Loads a BehaviorTree from a file path.
*	\param filename The file path
*	\return The uid of the Behavior Tree on SUCCESS, -1 on FAIL.
*/
BEET_API unsigned int BEET_LoadBehaviorTreeFromFile(const char* filename, BEET_bool debug);

BEET_API void BEET_ExecuteBehaviorTree(unsigned int id); // DEPRECATED

/*
*	Get a list of all the nodes in a Behavior Tree of type 'Task' by their name. 
*	IMPORTANT! DestroyDequeue when finish to free the buffer memory.
*	\param1 Behavior Tree id.
*   \param2 Pointer to the dequeue list to be filled.
*	\return Number of names found.
*/
BEET_API int BEET_GetAllTasksNames(unsigned int btId, dequeue* listNames); // DEPRECATED

/*
*	Sets the function to be used when the 'Task' node is running.
*	\param1 Function callback to be called.
*	\return Non-zero on success. 0 on failure.
*/
BEET_API int BEET_SetTaskCallbackFunc(beetCallbackFunc callback);

//--------------------------------------------------------------------------------
// Blackboard
//--------------------------------------------------------------------------------

BEET_API BEET_bool		BEET_BBGetBool(unsigned int btId, const char* varName);
BEET_API int			BEET_BBGetInt(unsigned int btId, const char* varName);
BEET_API float			BEET_BBGetFloat(unsigned int btId, const char* varName);
BEET_API const char*	BEET_BBGetString(unsigned int btId, const char* varName);

BEET_API BEET_bool BEET_BBSetBool(unsigned int btId, const char* varName, BEET_bool value);
BEET_API BEET_bool BEET_BBSetInt(unsigned int btId, const char* varName, int value);
BEET_API BEET_bool BEET_BBSetFloat(unsigned int btId, const char* varName, float value);
BEET_API BEET_bool BEET_BBSetString(unsigned int btId, const char* varName, const char* value);

//--------------------------------------------------------------------------------
// Utils
//--------------------------------------------------------------------------------

// Get the number of Behavior Trees loaded in memory.
BEET_API size_t	BEET_BehaviorTreeCount();

// Get the current context
BEET_API BeetContext* BeeT_GetContext();

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