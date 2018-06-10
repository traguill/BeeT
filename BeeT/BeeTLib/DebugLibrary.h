#pragma once

#include <windows.h>
#include "beet_std.h"
#include "BeeT_node.h"

#define SAY(x) OutputDebugString(x"\n");
#define SAYB(x) OutputDebugString(x);

void TO_DEBUG_PrintRunningNodes(dequeue* d, int isEnd)
{
	if(isEnd)  {SAY("-- End ---------"); } else {SAY("-- Start -------");}
	
	node_deq* item = dequeue_head(d);
	while (item)
	{
		BeeT_Node* n = (BeeT_Node*)item->data;
		if (n)
		{
			char toSay[256];
			
			const char* type;
			switch (n->type)
			{
			case NT_ROOT:
				type = "Root";
				break;
			case NT_SEQUENCE:
				type = "Sequence";
				break;
			case NT_SELECTOR:
				type = "Selector";
				break;
			case NT_PARALLEL:
				type = "Parallel";
				break;
			case NT_TASK:
				type = "Task";
				break;
			case NT_WAIT:
				type = "Wait";
				break;
			default:
				type = "NO TYPE";
				break;
			}

			const char* status;
			switch (n->status)
			{
			case NS_INVALID:
				status = "Invalid";
				break;
			case NS_RUNNING:
				status = "Running";
				break;
			case NS_FAILURE:
				status = "Failure";
				break;
			case NS_SUCCESS:
				status = "Success";
				break;
			case NS_SUSPENDED:
				status = "SUSPENDED";
				break;
			default:
				status = "NO STATUS";
				break;
			}
			sprintf(toSay, "%s (%i): %s\n", type, n->id, status);
			SAYB(toSay);
		}
		item = item->next;
	}
}