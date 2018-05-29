#include "BeeT_DBG_behaviortree.h"
#include "BeeT_behaviortree.h"
#include "BeeT_blackboard.h"
#include "BeeT_serializer.h"

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

double GetTimestamp(clock_t startTime)
{
	return (double)(clock() - startTime) / CLOCKS_PER_SEC;
}

void BeeT_dBT_bbBool(BeeT_dBT * bt, BBVar * var, BEET_bool newValue)
{
	BeeT_sBBVar* sample = BeeT_dBT_InitsBBVar();
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
	BeeT_sBBVar* sample = BeeT_dBT_InitsBBVar();
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
	BeeT_sBBVar* sample = BeeT_dBT_InitsBBVar();
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
	BeeT_sBBVar* sample = BeeT_dBT_InitsBBVar();
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

BeeT_Serializer * BeeT_dSample_Serialize(BeeT_dSample * sample)
{
	BeeT_Serializer* data = BeeT_Serializer_Create();

	BeeT_Serializer_AppendInt(data, "type", (int)sample->type);
	BeeT_Serializer_AppendDouble(data, "time", sample->time);

	switch (sample->type)
	{
		case BBVAR_CHANGED:
			BeeT_dBT_BBVarSerialize(data, (BeeT_sBBVar*)sample);
			break;
		case NODE_RETURNS:
			break;
		case NEW_CURRENT_NODE:
			break;
		case DECORATOR_CONDITION:
			break;
	}

	return data;
}

BeeT_sBBVar * BeeT_dBT_InitsBBVar()
{
	BeeT_sBBVar* sVar = (BeeT_sBBVar*)BEET_malloc(sizeof(BeeT_sBBVar));
	sVar->sample.time = clock();
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
