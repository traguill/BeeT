#ifndef __BEET_H__
#define __BEET_H__

#include "beet_std.h"

#include "beet_begin.h"

#include "BeeT_node.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------------
typedef enum NodeFunction
{
	NF_ONINIT,
	NF_UPDATE,
	NF_ONFINISH
}NodeFunction;

typedef NodeStatus(*beetCallbackFunc)(unsigned int btId, const char* taskId, NodeFunction func, const BBVar* extraData);

typedef struct
{
	BEET_bool initialized;
	struct BeeT_BehaviorTree** trees;
	int treesSize;
	int treesCapacity;
	float dt;
	beetCallbackFunc taskCallbackFunc;
	dequeue* treesToRemove;
}BeetContext;

//--------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------

BEET_API void		BEET_Init();
BEET_API void		BEET_Shutdown();

BEET_API BEET_bool	BEET_InitDebugger(const char* ip, int port);

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

/*
*	Sets the function to be used when the 'Task' node is running.
*	\param1 Function callback to be called.
*	\return Non-zero on success. 0 on failure.
*/
BEET_API int BEET_SetTaskCallbackFunc(beetCallbackFunc callback);

// Pauses the execution of a behavior tree. Returns non-zero on success, otherwise returns 0.
// If the behavior tree was already paused this will return success.
BEET_API int BEET_PauseBehaviorTree(unsigned int btId);

// Resumes the execution of a behavior tree. Returns non-zero on success, otherwise returns 0
// If the behavior tree was already running this will return success.
BEET_API int BEET_ResumeBehaviorTree(unsigned int btId);

// Stops the execution of a behavior tree and cleans it from memory. Returns non-zero on success, otherwise returns 0
BEET_API int BEET_CloseBehaviorTree(unsigned int btId);

BEET_API BEET_bool BEET_BehaviorTreeIsPaused(unsigned int btId);

//--------------------------------------------------------------------------------
// Blackboard
//--------------------------------------------------------------------------------

BEET_API BEET_bool		BEET_BBGetBool(unsigned int btId, const char* varName);
BEET_API BEET_bool		BEET_BBSetBool(unsigned int btId, const char* varName, BEET_bool value);

BEET_API int			BEET_BBGetInt(unsigned int btId, const char* varName);
BEET_API BEET_bool		BEET_BBSetInt(unsigned int btId, const char* varName, int value);

BEET_API float			BEET_BBGetFloat(unsigned int btId, const char* varName);
BEET_API BEET_bool		BEET_BBSetFloat(unsigned int btId, const char* varName, float value);

BEET_API const char*	BEET_BBGetString(unsigned int btId, const char* varName);
BEET_API BEET_bool		BEET_BBSetString(unsigned int btId, const char* varName, const char* value);

BEET_API float2			BEET_BBGetVector2(unsigned int btId, const char* varName);
BEET_API BEET_bool		BEET_BBSetVector2(unsigned int btId, const char* varName, const float2* value);
BEET_API BEET_bool		BEET_BBSetVector2Param(unsigned int btId, const char* varName, float x, float y);

BEET_API float3			BEET_BBGetVector3(unsigned int btId, const char* varName);
BEET_API BEET_bool		BEET_BBSetVector3(unsigned int btId, const char* varName, const float3* value);
BEET_API BEET_bool		BEET_BBSetVector3Param(unsigned int btId, const char* varName, float x, float y, float z);


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
BEET_API int BEET_SetMemoryFunctions(BeeT_Malloc_Function malloc_func, BeeT_Free_Function free_func, BeeT_Realloc_Function realloc_func, BeeT_Calloc_Function calloc_func);

#ifdef __cplusplus
}
#endif
#include "beet_end.h"
#endif // BEET_H