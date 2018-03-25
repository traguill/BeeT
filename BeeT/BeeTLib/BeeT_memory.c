#include "beet.h"

#include <stdlib.h>

#define initial_malloc malloc
#define initial_free free
#define initial_realloc realloc

// Memory functions used by BeeT that can be replaced by the application
static struct
{
	BeeT_Malloc_Function malloc_func;
	BeeT_Free_Function free_func;
	BeeT_Realloc_Function realloc_func;
} b_mem = {
	initial_malloc, initial_free, initial_realloc
};

BEET_API int BEET_SetMemoryFunctions(BeeT_Malloc_Function malloc_func, BeeT_Free_Function free_func, BeeT_Realloc_Function realloc_func)
{
	if (!malloc_func || !free_func || !realloc_func)
		return 0;

	b_mem.malloc_func = malloc_func;
	b_mem.free_func = free_func;
	b_mem.realloc_func = realloc_func;

	return 1;
}

void* BEET_malloc(size_t size)
{
	void *mem;
	if (!size)
		size = 1;
	mem = b_mem.malloc_func(size);
	return mem;
}

void BEET_free(void* ptr)
{
	if (!ptr)
		return;
	b_mem.free_func(ptr);
}

void* BEET_realloc(void* ptr, size_t size)
{
	void *mem;
	if (!ptr && !size)
		size = 1;
	mem = b_mem.realloc_func(ptr, size);
	return mem;
}