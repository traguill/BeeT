#ifndef __BEET_DEBUGGER_H__
#define __BEET_DEBUGGER_H__

#include "beet_std.h"

typedef struct BeeT_debugger BeeT_debugger;
struct BeeT_debugger
{
	BEET_bool initialized;
	BEET_bool enabled;
	struct BeeT_network* nw;
};

BEET_bool BeeT_Debugger_Init(BeeT_debugger* debugger, int port);
void BeeT_Debugger_Tick(BeeT_debugger* debugger);



#endif // !__BEET_DEBUGGER_H__
