#ifndef __BEET_DECORATOR_H__
#define __BEET_DECORATOR_H__

#include "BeeT_blackboard.h"
#include "BeeT_serializer.h"

typedef struct BeeT_decorator BeeT_decorator;
struct BeeT_decorator
{
	BBVar* varA;
	void* varB;

	BEET_bool(*Pass)(BeeT_decorator*);
};

BeeT_decorator* BeeT_Decorator_Init(BeeT_Serializer* data, BeeT_Blackboard* bb);
void BeeT_Decorator_Destroy(BeeT_decorator* self);

#endif // !__BEET_DECORATOR_H__

