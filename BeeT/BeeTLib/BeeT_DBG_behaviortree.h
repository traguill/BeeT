#ifndef __BEET_DBG_BEHAVIORTREE_H__
#define __BEET_DBG_BEHAVIORTREE_H__

#include "beet_std.h"
#include <time.h>
#include "../SharedData/SharedEnums.h"

typedef struct BeeT_dBT BeeT_dBT;
struct BeeT_dBT
{
	int uid;
	BEET_bool initialized;

	void* btBuffer; // Only used at the beginning to send the BT structure
	unsigned int dataToSendSize;
	dequeue* samples; // Changes of the BT that will be sent this Tick
	clock_t startTime;
	struct BeeT_Node* currentNode;
};

BeeT_dBT*		BeeT_dBT_Init			(const char* buffer, unsigned int size);	// Constructor
BEET_bool		BeeT_dBT_HasDataToSend	(const BeeT_dBT* bt);						// Returns BEET_TRUE if there is new data to send, BEET_FALSE otherwise
int				BeeT_dBT_GetSampleData	(BeeT_dBT* bt, char** buf);					// Returns a buffer with the data ready to be sent. After calling this, dataToSendSize contains the buffer size.
void			BeeT_dBT_ClearSampleData(BeeT_dBT* bt);
float GetTimestamp(clock_t startTime); // Helper


// Blackboard variables change their value
void BeeT_dBT_bbBool(BeeT_dBT* bt, struct BBVar* var, BEET_bool newValue);
void BeeT_dBT_bbInt(BeeT_dBT* bt, struct BBVar* var, int newValue);
void BeeT_dBT_bbFloat(BeeT_dBT* bt, struct BBVar* var, float newValue);
void BeeT_dBT_bbString(BeeT_dBT* bt, struct BBVar* var, const char* newValue);

// New current node
void BeeT_dBT_NewCurrentNode(BeeT_dBT* bt, struct BeeT_Node* newCurrent);

// Node return status
void BeeT_dBT_NodeReturnStatus(BeeT_dBT* bt, struct BeeT_Node* node, enum NodeStatus newStatus);

// Behavior Tree ended
void BeeT_dBT_BTEnd(BeeT_dBT* bt);

// Information to send to the Editor
// --------------------------------------------------------------------------------
// Samples
// --------------------------------------------------------------------------------

typedef struct BeeT_dSample BeeT_dSample;
struct BeeT_dSample
{
	SampleType type;
	float time;
};
BeeT_dSample* BeeT_dBT_InitSample(SampleType type, float time);
struct BeeT_Serializer* BeeT_dSample_Serialize(BeeT_dSample* sample);

// Sample Types
// --------------------------------------------------------------------------------

typedef struct BeeT_sBBVar
{
	BeeT_dSample sample;
	enum BBVarType varType;
	char* name;
	void* oldValue;
	void* newValue;
}BeeT_sBBVar;

BeeT_sBBVar* BeeT_dBT_InitsBBVar(clock_t startTime);
void BeeT_dBT_BBVarSerialize(struct BeeT_Serializer* data, BeeT_sBBVar* sample);

typedef struct BeeT_sNewCurrent
{
	BeeT_dSample sample;
	int oldCurrent;
	int newCurrent;
}BeeT_sNewCurrent;

BeeT_sNewCurrent* BeeT_dBT_InitsNewCurrent(clock_t startTime, int oldCurrent, int newCurrent);
void BeeT_dBT_sNewCurrentSerialize(struct BeeT_Serializer* data, BeeT_sNewCurrent* sample);

typedef struct BeeT_sNodeReturn
{
	BeeT_dSample sample;
	int nodeId;
	enum NodeStatus oldStatus;
	enum NodeStatus newStatus;
}BeeT_sNodeReturn;

BeeT_sNodeReturn* BeeT_dBT_InitsNodeReturn(clock_t startTime, int nodeId, enum NodeStatus oldStatus, enum NodeStatus newStatus);
void BeeT_dBT_sNodeReturnSerialize(struct BeeT_Serializer* data, BeeT_sNodeReturn* sample);

typedef struct BeeT_dSample BeeT_sBTEnd;
#endif // !__BEET_DBG_BEHAVIORTREE_H__

