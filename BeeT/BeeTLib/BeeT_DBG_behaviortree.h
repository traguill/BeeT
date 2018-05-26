#ifndef __BEET_DBG_BEHAVIORTREE_H__
#define __BEET_DBG_BEHAVIORTREE_H__

#include "beet_std.h"

typedef struct BeeT_DBG_BT BeeT_DBG_BT;
struct BeeT_DBG_BT
{
	unsigned int uid;
	BEET_bool initialized;
	// orders to send?
	void* btBuffer; // Only used at the beginning to send the BT structure
	unsigned int dataToSendSize;
};

BeeT_DBG_BT* BeeT_DGB_BT_Init(unsigned int uid, const char* buffer, unsigned int size);
#endif // !__BEET_DBG_BEHAVIORTREE_H__

