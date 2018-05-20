#include "BeeT_debugger.h"
#include "BeeT_network.h"


BEET_bool BeeT_Debugger_Init(BeeT_debugger * debugger, int port) // TODO: Do clean up of socketList
{
	debugger->initialized = BEET_TRUE;
	debugger->nw = BeeT_NW_Init(port);
	return debugger->nw != NULL ? BEET_TRUE : BEET_FALSE;
}

void BeeT_Debugger_Tick(BeeT_debugger * debugger)
{
	BeeT_NW_Tick(debugger->nw);
}