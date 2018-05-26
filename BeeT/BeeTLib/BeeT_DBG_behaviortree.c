#include "BeeT_DBG_behaviortree.h"

BeeT_DBG_BT * BeeT_DGB_BT_Init(unsigned int uid, const char* buffer, unsigned int size)
{
	BeeT_DBG_BT* bt = (BeeT_DBG_BT*)BEET_malloc(sizeof(BeeT_DBG_BT));
	bt->uid = uid;

	bt->btBuffer = BEET_malloc(size);
	BEET_memcpy(bt->btBuffer, buffer, size);
	bt->dataToSendSize = size;
	return bt;
}
