#include "BeeT_debugger.h"
#include "BeeT_network.h"
#include "BeeT_DBG_behaviortree.h"


BEET_bool BeeT_Debugger_Init(BeeT_debugger * debugger, int port) // TODO: Do clean up of socketList
{
	debugger->initialized = BEET_TRUE;
	debugger->nw = BeeT_NW_Init(port);
	debugger->BTs = InitDequeue();
	return debugger->nw != NULL ? BEET_TRUE : BEET_FALSE;
}

void BeeT_Debugger_Tick(BeeT_debugger * debugger)
{
	BeeT_NW_Tick(debugger->nw);
}

void BeeT_Debugger_LoadBT(BeeT_debugger * debugger, const char * buffer, unsigned int bufSize, unsigned int uid)
{
	BeeT_DBG_BT* dbg_bt = BeeT_DGB_BT_Init(uid, buffer, bufSize);

	BEET_bool result = BeeT_NW_OpenSocket(debugger->nw, dbg_bt);
	if (result == BEET_FALSE)
	{
		// TODO: REMOVE THE BT CREATED
		// print a error msg: "not enough sockets to load a new bt"
	}
	dequeue_push_back(debugger->BTs, dbg_bt);

}