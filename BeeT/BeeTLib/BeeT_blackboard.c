#include "BeeT_blackboard.h"
#include <stdio.h>

typedef enum BBVarType BBVarType;
enum BBVarType
{
	BV_BOOL,
	BV_INT,
	BV_FLOAT,
	BV_STRING
};

typedef struct BBVar BBVar;
struct BBVar
{
	const char* name;
	BBVarType type;
	void* data;
};

BeeT_Blackboard* BeeT_Blackboard_Init(const BeeT_Serializer * data)
{
	BeeT_Blackboard* bb = (BeeT_Blackboard*)BEET_malloc(sizeof(BeeT_Blackboard));
	
	bb->vars = InitDequeue();

	int numVars = (int)BeeT_Serializer__GetArraySize(data, "blackboard");

	for (int i = 0; i < numVars; ++i)
	{
		BeeT_Serializer* varData = BeeT_Serializer__GetArray(data, "blackboard", i);
		BBVar* var = (BBVar*)BEET_malloc(sizeof(BBVar));
	
		const char* name = BeeT_Serializer__GetString(varData, "name");
		unsigned int length = strlen(name) + 1;
		var->name = (const char*)BEET_malloc(length);
		strcpy(var->name, name);

		var->type = (BBVarType)BeeT_Serializer__GetInt(varData, "type");

		switch (var->type)
		{
		case BV_BOOL:
			var->data = BEET_malloc(sizeof(BEET_bool));
			*((BEET_bool*)var->data) = BeeT_Serializer__GetBool(varData, "value");	
			break;
		case BV_INT:
			var->data = BEET_malloc(sizeof(int));
			*((int*)var->data) = BeeT_Serializer__GetInt(varData, "value");
			break;
		case BV_FLOAT:
			var->data = BEET_malloc(sizeof(float));
			*((float*)(var->data)) = BeeT_Serializer__GetFloat(varData, "value");
			break;
		case BV_STRING:
			{
				const char* bvName = BeeT_Serializer__GetString(varData, "value");
				unsigned int bvLength = strlen(bvName) + 1;
				var->data = BEET_malloc(bvLength);
				strcpy(var->data, bvName);
			}	
			break;
		}
		BEET_free(varData);
		dequeue_push_back(bb->vars, var);
	}

	return bb;
}

void BeeT_Blackboard_Destroy(BeeT_Blackboard * self)
{
	if (self->vars) //TODO: remove inside content of var. Actual implementation has memory leaks
		DestroyDequeue(self->vars);
	BEET_free(self);
}
