#include "BeeT_DBG_behaviortree.h"
#include "BeeT_behaviortree.h"
#include "BeeT_blackboard.h"
#include "BeeT_serializer.h"
#include "BeeT_node.h"

BeeT_dBT * BeeT_dBT_Init(const char* buffer, unsigned int size)
{
	BeeT_dBT* dbg_bt = (BeeT_dBT*)BEET_malloc(sizeof(BeeT_dBT));
	dbg_bt->uid = 0;

	dbg_bt->btBuffer = BEET_malloc(size);
	BEET_memcpy(dbg_bt->btBuffer, buffer, size);
	dbg_bt->dataToSendSize = size;
	dbg_bt->initialized = BEET_FALSE;
	dbg_bt->samples = InitDequeue();
	dbg_bt->startTime = clock();
	dbg_bt->currentNode = NULL;
	return dbg_bt;
}

BEET_bool BeeT_dBT_HasDataToSend(const BeeT_dBT * bt)
{
	return !dequeue_is_empty(bt->samples);
}

int BeeT_dBT_GetSampleData(BeeT_dBT* bt, char** buf)
{
	BeeT_Serializer* data = BeeT_Serializer_Create();
	BeeT_Serializer_AppendInt(data, "uid", bt->uid);
	BeeT_Serializer_AppendArray(data, "samples");

	node_deq* it = bt->samples->head;
	while (it != NULL)
	{
		BeeT_Serializer* sData = BeeT_dSample_Serialize((BeeT_dSample*)it->data);
		BeeT_Serializer_AppendArrayValue(data, sData);
		it = it->next;
	}
	int size = BeeT_Serializer_Serialize(data, buf);
	BeeT_Serializer_Destroy(data);
	return size;
}

void BeeT_dBT_ClearSampleData(BeeT_dBT * bt)
{
	dequeue_clear(bt->samples); // TODO: Clear every sample individually
}

float GetTimestamp(clock_t startTime)
{
	return ((float)(clock() - startTime) / (float)CLOCKS_PER_SEC);
}

void BeeT_dBT_bbBool(BeeT_dBT * bt, BBVar * var, BEET_bool newValue)
{
	BeeT_sBBVar* sample = BeeT_dBT_InitsBBVar(bt->startTime);
	sample->varType = var->type;
	sample->oldValue = BEET_malloc(sizeof(BEET_bool));
	sample->newValue = BEET_malloc(sizeof(BEET_bool));
	BEET_memcpy(sample->oldValue, var->data, sizeof(BEET_bool));
	BEET_memcpy(sample->newValue, &newValue, sizeof(BEET_bool));
	unsigned int length = strlen(var->name) + 1;
	sample->name = (char*)BEET_malloc(length);
	strcpy(sample->name, var->name);
	dequeue_push_back(bt->samples, sample);
}

void BeeT_dBT_bbInt(BeeT_dBT * bt, BBVar * var, int newValue)
{
	BeeT_sBBVar* sample = BeeT_dBT_InitsBBVar(bt->startTime);
	sample->varType = var->type;
	sample->oldValue = BEET_malloc(sizeof(int));
	sample->newValue = BEET_malloc(sizeof(int));
	BEET_memcpy(sample->oldValue, var->data, sizeof(int));
	BEET_memcpy(sample->newValue, &newValue, sizeof(int));
	unsigned int length = strlen(var->name) + 1;
	sample->name = (char*)BEET_malloc(length);
	strcpy(sample->name, var->name);
	dequeue_push_back(bt->samples, sample);
}

void BeeT_dBT_bbFloat(BeeT_dBT * bt, BBVar * var, float newValue)
{
	BeeT_sBBVar* sample = BeeT_dBT_InitsBBVar(bt->startTime);
	sample->varType = var->type;
	sample->oldValue = BEET_malloc(sizeof(float));
	sample->newValue = BEET_malloc(sizeof(float));
	BEET_memcpy(sample->oldValue, var->data, sizeof(float));
	BEET_memcpy(sample->newValue, &newValue, sizeof(float));
	unsigned int length = strlen(var->name) + 1;
	sample->name = (char*)BEET_malloc(length);
	strcpy(sample->name, var->name);
	dequeue_push_back(bt->samples, sample);
}

void BeeT_dBT_bbString(BeeT_dBT * bt, BBVar * var, const char * newValue)
{
	BeeT_sBBVar* sample = BeeT_dBT_InitsBBVar(bt->startTime);
	sample->varType = var->type;
	unsigned int length;

	length = strlen((const char*)var->data) + 1;
	sample->oldValue = (char*)BEET_malloc(length);
	strcpy((char*)sample->oldValue, (const char*)var->data);
	
	length = strlen((const char*)newValue) + 1;
	sample->newValue = (char*)BEET_malloc(length);
	strcpy((char*)sample->newValue, (const char*)newValue);

	length = strlen(var->name) + 1;
	sample->name = (char*)BEET_malloc(length);
	strcpy(sample->name, var->name);
	dequeue_push_back(bt->samples, sample);
}

void BeeT_dBT_NewCurrentNode(BeeT_dBT * bt, BeeT_Node * newCurrent)
{
	int oldId = bt->currentNode ? bt->currentNode->id : -1;
	int newId = newCurrent ? newCurrent->id : -1;
	BeeT_sNewCurrent* sample = BeeT_dBT_InitsNewCurrent(bt->startTime, oldId, newId);
	dequeue_push_back(bt->samples, sample);
	bt->currentNode = newCurrent;
}

void BeeT_dBT_NodeReturnStatus(BeeT_dBT * bt, BeeT_Node * node, NodeStatus newStatus)
{
	BeeT_sNodeReturn* sample = BeeT_dBT_InitsNodeReturn(bt->startTime, node->id, node->status, newStatus);
	dequeue_push_back(bt->samples, sample);
}

void BeeT_dBT_BTEnd(BeeT_dBT * bt)
{
	BeeT_sBTEnd* sample = BeeT_dBT_InitSample(BT_END, GetTimestamp(bt->startTime));
	dequeue_push_back(bt->samples, sample);
}

BeeT_dSample * BeeT_dBT_InitSample(SampleType type, float time)
{
	BeeT_dSample* sample = (BeeT_dSample*)BEET_malloc(sizeof(BeeT_dSample));
	sample->type = type;
	sample->time = time;
	return sample;
}

BeeT_Serializer * BeeT_dSample_Serialize(BeeT_dSample * sample)
{
	BeeT_Serializer* data = BeeT_Serializer_Create();

	BeeT_Serializer_AppendInt(data, "type", (int)sample->type);
	BeeT_Serializer_AppendFloat(data, "timestamp", sample->time);

	switch (sample->type)
	{
		case BBVAR_CHANGED:
			BeeT_dBT_BBVarSerialize(data, (BeeT_sBBVar*)sample);
			break;
		case NODE_RETURNS:
			BeeT_dBT_sNodeReturnSerialize(data, (BeeT_sNodeReturn*)sample);
			break;
		case NEW_CURRENT_NODE:
			BeeT_dBT_sNewCurrentSerialize(data, (BeeT_sNewCurrent*)sample);
			break;
		case DECORATOR_CONDITION:
			break;
		case BT_END:
			// No need to do anything
			break;
	}

	return data;
}

BeeT_sBBVar * BeeT_dBT_InitsBBVar(clock_t startTime)
{
	BeeT_sBBVar* sVar = (BeeT_sBBVar*)BEET_malloc(sizeof(BeeT_sBBVar));
	sVar->sample.time = GetTimestamp(startTime);
	sVar->sample.type = BBVAR_CHANGED;
	return sVar;
}

void BeeT_dBT_BBVarSerialize(BeeT_Serializer * data, BeeT_sBBVar * sample)
{
	BeeT_Serializer_AppendInt(data, "varType", (int)sample->varType);
	BeeT_Serializer_AppendString(data, "name", sample->name);

	switch (sample->varType)
	{
		case BV_BOOL:
			BeeT_Serializer_AppendBool(data, "oldValue", *(BEET_bool*)sample->oldValue);
			BeeT_Serializer_AppendBool(data, "newValue", *(BEET_bool*)sample->newValue);
			break;
		case BV_INT:
			BeeT_Serializer_AppendInt(data, "oldValue", *(int*)sample->oldValue);
			BeeT_Serializer_AppendInt(data, "newValue", *(int*)sample->newValue);
			break;
		case BV_FLOAT:
			BeeT_Serializer_AppendFloat(data, "oldValue", *(float*)sample->oldValue);
			BeeT_Serializer_AppendFloat(data, "newValue", *(float*)sample->newValue);
			break;
		case BV_STRING:
			BeeT_Serializer_AppendString(data, "oldValue", (char*)sample->oldValue);
			BeeT_Serializer_AppendString(data, "newValue", (char*)sample->newValue);
			break;
	}
}

BeeT_sNewCurrent * BeeT_dBT_InitsNewCurrent(clock_t startTime, int oldCurrent, int newCurrent)
{
	BeeT_sNewCurrent* sNC = (BeeT_sNewCurrent*)BEET_malloc(sizeof(BeeT_sNewCurrent));
	sNC->sample.type = NEW_CURRENT_NODE;
	sNC->sample.time = GetTimestamp(startTime);

	sNC->newCurrent = newCurrent;
	sNC->oldCurrent = oldCurrent;

	return sNC;
}

void BeeT_dBT_sNewCurrentSerialize(BeeT_Serializer * data, BeeT_sNewCurrent * sample)
{
	BeeT_Serializer_AppendInt(data, "oldCurrent", sample->oldCurrent);
	BeeT_Serializer_AppendInt(data, "newCurrent", sample->newCurrent);
}

BeeT_sNodeReturn * BeeT_dBT_InitsNodeReturn(clock_t startTime, int nodeId, NodeStatus oldStatus, NodeStatus newStatus)
{
	BeeT_sNodeReturn* sNR = (BeeT_sNodeReturn*)BEET_malloc(sizeof(BeeT_sNodeReturn));
	sNR->sample.type = NODE_RETURNS;
	sNR->sample.time = GetTimestamp(startTime);
	sNR->nodeId = nodeId;
	sNR->newStatus = newStatus;
	sNR->oldStatus = oldStatus;
	return sNR;
}

void BeeT_dBT_sNodeReturnSerialize(BeeT_Serializer * data, BeeT_sNodeReturn * sample)
{
	BeeT_Serializer_AppendInt(data, "oldStatus", (int)sample->oldStatus);
	BeeT_Serializer_AppendInt(data, "newStatus", (int)sample->newStatus);
	BeeT_Serializer_AppendInt(data, "nodeId", sample->nodeId);
}
