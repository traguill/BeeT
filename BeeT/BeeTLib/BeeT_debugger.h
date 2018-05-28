#ifndef __BEET_DEBUGGER_H__
#define __BEET_DEBUGGER_H__

#include "beet_std.h"

typedef struct BeeT_debugger BeeT_debugger;
struct BeeT_debugger
{
	BEET_bool initialized;
	struct BeeT_network* nw;
	dequeue* BTs; //TODO: replace it with a map
};

BEET_bool BeeT_Debugger_Init(BeeT_debugger* debugger, int port);
void BeeT_Debugger_Tick(BeeT_debugger* debugger);

struct BeeT_dBT* BeeT_Debugger_LoadBT(BeeT_debugger* debugger, const char* buffer, unsigned int buffSize, unsigned int uid);
#endif // !__BEET_DEBUGGER_H__
