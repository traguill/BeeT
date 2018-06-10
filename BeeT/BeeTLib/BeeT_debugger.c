#include "BeeT_debugger.h"
#include "BeeT_network.h"
#include "BeeT_DBG_behaviortree.h"
#include "BeeT_behaviortree.h"


BEET_bool BeeT_Debugger_Init(BeeT_debugger * debugger, const char* ip, int port)
{
	debugger->initialized = BEET_TRUE;
	debugger->nw = BeeT_NW_Init(ip, port);
	debugger->BTs = InitDequeue();
	return debugger->nw != NULL ? BEET_TRUE : BEET_FALSE;
}

void BeeT_Debugger_Tick(BeeT_debugger * debugger)
{
	BeeT_NW_Tick(debugger->nw);
}

void BeeT_Debugger_Cleanup(BeeT_debugger * debugger)
{
	if (debugger->BTs)
	{
		node_deq* item = dequeue_head(debugger->BTs);
		while (item)
		{
			BeeT_dBT* dbt = (BeeT_dBT*)item->data;
			BeeT_dBT_Cleanup(dbt);
			BEET_free(dbt);
			item = item->next;
		}
		DestroyDequeue(debugger->BTs);
	}
	if (debugger->nw)
	{
		BeeT_NW_Cleanup(debugger->nw);
		BEET_free(debugger->nw);
	}
}

BeeT_dBT* BeeT_Debugger_LoadBT(BeeT_debugger * debugger, const char * buffer, unsigned int bufSize)
{
	BeeT_dBT* dbg_bt = BeeT_dBT_Init(buffer, bufSize);

	BEET_bool result = BeeT_NW_OpenSocket(debugger->nw, dbg_bt);
	if (result == BEET_FALSE)
	{
		// TODO: REMOVE THE BT CREATED
		// print a error msg: "not enough sockets to load a new bt"
	}
	dequeue_push_back(debugger->BTs, dbg_bt);
	return dbg_bt;
}