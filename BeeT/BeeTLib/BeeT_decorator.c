#include "BeeT_decorator.h"

// Forward declarations
BEET_bool Pass_BoolIsFalse(BeeT_decorator* dec);
BEET_bool Pass_BoolIsTrue(BeeT_decorator* dec);
BEET_bool Pass_IntEqual(BeeT_decorator* dec);
BEET_bool Pass_IntNotEqual(BeeT_decorator* dec);
BEET_bool Pass_IntGreater(BeeT_decorator* dec);
BEET_bool Pass_IntLess(BeeT_decorator* dec);
BEET_bool Pass_IntEqualGreater(BeeT_decorator* dec);
BEET_bool Pass_IntEqualLess(BeeT_decorator* dec);
BEET_bool Pass_FloatEqual(BeeT_decorator* dec);
BEET_bool Pass_FloatNotEqual(BeeT_decorator* dec);
BEET_bool Pass_FloatGreater(BeeT_decorator* dec);
BEET_bool Pass_FloatLess(BeeT_decorator* dec);
BEET_bool Pass_FloatEqualGreater(BeeT_decorator* dec);
BEET_bool Pass_FloatEqualLess(BeeT_decorator* dec);
BEET_bool Pass_StringEqual(BeeT_decorator* dec);
BEET_bool Pass_StringNotEqual(BeeT_decorator* dec);
BEET_bool Pass_StringContains(BeeT_decorator* dec);
BEET_bool Pass_StringNotContains(BeeT_decorator* dec);

BeeT_decorator * BeeT_Decorator_Init(BeeT_Serializer * data, BeeT_Blackboard * bb, int nodeId)
{
	BeeT_decorator* dec = BEET_malloc(sizeof(BeeT_decorator));
	dec->id = BeeT_Serializer_GetInt(data, "uid");
	dec->nodeId = nodeId;
	const char* varAName = BeeT_Serializer_GetString(data, "var");
	dec->varA = bb->FindVar(bb, varAName);
	dec->checkAlways = BeeT_Serializer_GetBool(data, "checkEveryFrame");
	int compareOption = BeeT_Serializer_GetInt(data, "option");
	dec->varB = NULL;
	if (dec->varA)
	{
		switch (dec->varA->type)
		{
		case BV_BOOL:
			if (compareOption == 0)
				dec->Pass = &Pass_BoolIsFalse;
			else
				dec->Pass = &Pass_BoolIsTrue;
			break;
		case BV_INT:
			dec->varB = BEET_malloc(sizeof(int));
			*((int*)dec->varB) = BeeT_Serializer_GetInt(data, "var2");
			BEET_bool(*funcInt[6])(BeeT_decorator*) = { &Pass_IntEqual, &Pass_IntNotEqual, &Pass_IntGreater, &Pass_IntLess, &Pass_IntEqualGreater, &Pass_IntEqualLess };
			dec->Pass = funcInt[compareOption];
			break;
		case BV_FLOAT:
			dec->varB = BEET_malloc(sizeof(float));
			*((float*)(dec->varB)) = BeeT_Serializer_GetFloat(data, "var2");
			BEET_bool(*funcFloat[6])(BeeT_decorator*) = { &Pass_FloatEqual, &Pass_FloatNotEqual, &Pass_FloatGreater, &Pass_FloatLess, &Pass_FloatEqualGreater, &Pass_FloatEqualLess };
			dec->Pass = funcFloat[compareOption];
			break;
		case BV_STRING:
		{
			const char* bvName = BeeT_Serializer_GetString(data, "var2");
			unsigned int bvLength = strlen(bvName) + 1;
			dec->varB = BEET_malloc(bvLength);
			strcpy(dec->varB, bvName);
			BEET_bool(*funcStr[4])(BeeT_decorator*) = {&Pass_StringEqual, &Pass_StringNotEqual, &Pass_StringContains, &Pass_StringNotContains};
			dec->Pass = funcStr[compareOption];
		}
			break;
		}
	}

	return dec;
}

void BeeT_Decorator_Destroy(BeeT_decorator * self)
{
	if (self->varB)
		BEET_free(self->varB);

	BEET_free(self);
}

// Bool --------------------------------------------------------
BEET_bool Pass_BoolIsTrue(BeeT_decorator* dec)
{
	return *(BEET_bool*)dec->varA->data == BEET_TRUE;
}

BEET_bool Pass_BoolIsFalse(BeeT_decorator* dec)
{
	return *(BEET_bool*)dec->varA->data == BEET_FALSE;
}
// --------------------------------------------------------------


// Int ----------------------------------------------------------
BEET_bool Pass_IntEqual(BeeT_decorator* dec)
{
	return *(int*)dec->varA->data == *(int*)dec->varB;
}

BEET_bool Pass_IntNotEqual(BeeT_decorator* dec)
{
	return *(int*)dec->varA->data != *(int*)dec->varB;
}

BEET_bool Pass_IntGreater(BeeT_decorator* dec)
{
	return *(int*)dec->varA->data > *(int*)dec->varB;
}

BEET_bool Pass_IntLess(BeeT_decorator* dec)
{
	return *(int*)dec->varA->data < *(int*)dec->varB;
}

BEET_bool Pass_IntEqualGreater(BeeT_decorator* dec)
{
	return *(int*)dec->varA->data >= *(int*)dec->varB;
}

BEET_bool Pass_IntEqualLess(BeeT_decorator* dec)
{
	return *(int*)dec->varA->data <= *(int*)dec->varB;
}
// ------------------------------------------------------------


// Float ------------------------------------------------------
BEET_bool Pass_FloatEqual(BeeT_decorator* dec)
{
	return *(float*)dec->varA->data == *(float*)dec->varB;
}

BEET_bool Pass_FloatNotEqual(BeeT_decorator* dec)
{
	return *(float*)dec->varA->data != *(float*)dec->varB;
}

BEET_bool Pass_FloatGreater(BeeT_decorator* dec)
{
	return *(float*)dec->varA->data > *(float*)dec->varB;
}

BEET_bool Pass_FloatLess(BeeT_decorator* dec)
{
	return *(float*)dec->varA->data < *(float*)dec->varB;
}

BEET_bool Pass_FloatEqualGreater(BeeT_decorator* dec)
{
	return *(float*)dec->varA->data >= *(float*)dec->varB;
}

BEET_bool Pass_FloatEqualLess(BeeT_decorator* dec)
{
	return *(float*)dec->varA->data <= *(float*)dec->varB;
}
// ------------------------------------------------------------


// String -----------------------------------------------------
BEET_bool Pass_StringEqual(BeeT_decorator* dec)
{
	return strcmp(dec->varA->data, dec->varB) == 0;
}

BEET_bool Pass_StringNotEqual(BeeT_decorator* dec)
{
	return strcmp(dec->varA->data, dec->varB) != 0;
}

BEET_bool Pass_StringContains(BeeT_decorator* dec)
{
	return strstr(dec->varA->data, dec->varB) != NULL;
}

BEET_bool Pass_StringNotContains(BeeT_decorator* dec)
{
	return strstr(dec->varA->data, dec->varB) == NULL;
}
// ------------------------------------------------------------