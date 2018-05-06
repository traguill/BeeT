#ifndef __BEET_DEBUGGER_H__
#define __BEET_DEBUGGER_H__

#include "ThirdParty/SDL_net.h"
#include "beet_std.h"

typedef struct BeeT_debugger BeeT_debugger;
struct BeeT_debugger
{
	BEET_bool initialized;
	int port;
};

BEET_bool BeeT_Debugger_Init(BeeT_debugger* debugger, int port);


#endif // !__BEET_DEBUGGER_H__
