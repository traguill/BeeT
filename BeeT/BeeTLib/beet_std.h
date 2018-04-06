#ifndef __beet_std_h__
#define __beet_std_h__

#include <stdio.h>

typedef enum {
	BEET_FALSE = 0,
	BEET_TRUE = 1
}BEET_bool;

#ifndef BEET_ASSERT
#include <assert.h>
#define BEET_ASSERT(_EXPR)	assert(_EXPR)
#endif

#include "beet_begin.h"

#ifdef __cplusplus
extern "C" {
#endif

// Memory ---------------------------------------------------------
extern BEET_API void *BEET_malloc(size_t size);
extern BEET_API void  BEET_free(void* mem);
extern BEET_API void *BEET_realloc(void* mem, size_t size);

typedef void * (*BeeT_Malloc_Function)(size_t);
typedef void(*BeeT_Free_Function)(void*);
typedef void * (*BeeT_Realloc_Function)(void* mem, size_t size);
//------------------------------------------------------------------

// Dequeue ---------------------------------------------------------

typedef struct BeeT_dequeue dequeue;

struct BeeT_dequeue
{
	char* data;
	unsigned int size;
	unsigned int itemSize;
	unsigned int maxSize;
	int frontId;
	int backId;

	void (*push_back)(dequeue*, void*);
	void (*push_front)(dequeue*, void*);
	void (*pop_back)(dequeue*);
	void (*pop_front)(dequeue*);
	void* (*back)(dequeue*);
	void* (*front)(dequeue*);

};

BEET_API dequeue* InitDequeue(unsigned int itemSize);
BEET_API void DestroyDequeue(dequeue* deq);
// ------------------------------------------------------------------

#ifdef __cplusplus
}
#endif
#include "beet_end.h"



#endif // !__beet_std_h__
