#ifndef __BEET_INTERNAL_H__
#define __BEET_INTERNAL_H__

typedef enum{ 
	BEET_FALSE = 0, 
	BEET_TRUE = 1 
}BEET_bool;

#ifndef BEET_ASSERT
#include <assert.h>
#define BEET_ASSERT(_EXPR)	assert(_EXPR)
#endif

#endif // !__BEET_INTERNAL_H__

