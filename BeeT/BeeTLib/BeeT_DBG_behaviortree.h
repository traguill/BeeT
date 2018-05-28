#ifndef __BEET_DBG_BEHAVIORTREE_H__
#define __BEET_DBG_BEHAVIORTREE_H__

#include "beet_std.h"

typedef struct BeeT_dBT BeeT_dBT;
struct BeeT_dBT
{
	unsigned int uid;
	BEET_bool initialized;

	void* btBuffer; // Only used at the beginning to send the BT structure
	unsigned int dataToSendSize;
};

BeeT_dBT* BeeT_dBT_Init(unsigned int uid, const char* buffer, unsigned int size);

// Blackboard variables change their value
void BeeT_dBT_bbBool(BeeT_dBT* bt, struct BBVar* var, BEET_bool newValue);
void BeeT_dBT_bbInt(BeeT_dBT* bt, struct BBVar* var, int newValue);
void BeeT_dBT_bbFloat(BeeT_dBT* bt, struct BBVar* var, float newValue);
void BeeT_dBT_bbString(BeeT_dBT* bt, struct BBVar* var, const char* newValue);

// Information to send to the Editor
typedef enum SampleType SampleType;
enum SampleType
{
	BBVAR_CHANGED,
	NODE_RETURNS,
	NEW_CURRENT_NODE,
	DECORATOR_CONDITION
};

typedef struct BeeT_dSample BeeT_dSample;
struct BeeT_dSample
{
	SampleType type; //HERE!!!!
};

#endif // !__BEET_DBG_BEHAVIORTREE_H__

