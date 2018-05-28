#include "BeeT_DBG_behaviortree.h"
#include "BeeT_behaviortree.h"
#include "BeeT_blackboard.h"

BeeT_dBT * BeeT_dBT_Init(unsigned int uid, const char* buffer, unsigned int size)
{
	BeeT_dBT* dbg_bt = (BeeT_dBT*)BEET_malloc(sizeof(BeeT_dBT));
	dbg_bt->uid = uid;

	dbg_bt->btBuffer = BEET_malloc(size);
	BEET_memcpy(dbg_bt->btBuffer, buffer, size);
	dbg_bt->dataToSendSize = size;
	dbg_bt->initialized = BEET_FALSE;
	return dbg_bt;
}

void BeeT_dBT_bbBool(BeeT_dBT * bt, BBVar * var, BEET_bool newValue)
{
}

void BeeT_dBT_bbInt(BeeT_dBT * bt, BBVar * var, int newValue)
{
}

void BeeT_dBT_bbFloat(BeeT_dBT * bt, BBVar * var, float newValue)
{
}

void BeeT_dBT_bbString(BeeT_dBT * bt, BBVar * var, const char * newValue)
{
}
