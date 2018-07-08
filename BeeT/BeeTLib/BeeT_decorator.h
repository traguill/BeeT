#ifndef __BEET_DECORATOR_H__
#define __BEET_DECORATOR_H__

#include "BeeT_blackboard.h"
#include "BeeT_serializer.h"

typedef struct BeeT_decorator BeeT_decorator;
struct BeeT_decorator
{
	int id;
	int nodeId;
	BBVar* varA;
	void* varB;
	BEET_bool checkAlways;
	BEET_bool(*Pass)(BeeT_decorator*);
};

BeeT_decorator* BeeT_Decorator_Init(BeeT_Serializer* data, BeeT_Blackboard* bb, int nodeId);
void BeeT_Decorator_Destroy(BeeT_decorator* self);

// Comparison Functions

// Bool
BEET_bool Pass_BoolIsFalse(BeeT_decorator* dec);
BEET_bool Pass_BoolIsTrue(BeeT_decorator* dec);

// Int
BEET_bool Pass_IntEqual(BeeT_decorator* dec);
BEET_bool Pass_IntNotEqual(BeeT_decorator* dec);
BEET_bool Pass_IntGreater(BeeT_decorator* dec);
BEET_bool Pass_IntLess(BeeT_decorator* dec);
BEET_bool Pass_IntEqualGreater(BeeT_decorator* dec);
BEET_bool Pass_IntEqualLess(BeeT_decorator* dec);

// Float
BEET_bool Pass_FloatEqual(BeeT_decorator* dec);
BEET_bool Pass_FloatNotEqual(BeeT_decorator* dec);
BEET_bool Pass_FloatGreater(BeeT_decorator* dec);
BEET_bool Pass_FloatLess(BeeT_decorator* dec);
BEET_bool Pass_FloatEqualGreater(BeeT_decorator* dec);
BEET_bool Pass_FloatEqualLess(BeeT_decorator* dec);

// String
BEET_bool Pass_StringEqual(BeeT_decorator* dec);
BEET_bool Pass_StringNotEqual(BeeT_decorator* dec);
BEET_bool Pass_StringContains(BeeT_decorator* dec);
BEET_bool Pass_StringNotContains(BeeT_decorator* dec);

// Vector2
BEET_bool Pass_Vector2Equal(BeeT_decorator* dec);
BEET_bool Pass_Vector2NotEqual(BeeT_decorator* dec);

// Vector3
BEET_bool Pass_Vector3Equal(BeeT_decorator* dec);
BEET_bool Pass_Vector3NotEqual(BeeT_decorator* dec);

#endif // !__BEET_DECORATOR_H__

