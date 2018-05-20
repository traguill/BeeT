#ifndef __BEET_BLACKBOARD_H__
#define __BEET_BLACKBOARD_H__

#include "BeeT_serializer.h"
#include "beet_std.h"

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
	char* name;
	BBVarType type;
	void* data;
};

typedef struct BeeT_Blackboard BeeT_Blackboard;
struct BeeT_Blackboard
{
	dequeue* vars; // TODO: Change this for a map or hash table

	BBVar* (*FindVar)(BeeT_Blackboard* bb, const char* name);
};

BeeT_Blackboard* BeeT_Blackboard_Init(const BeeT_Serializer* data);
void BeeT_Blackboard_Destroy(BeeT_Blackboard* self);

#endif // !__BEET_BLACKBOARD_H__