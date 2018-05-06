#include "BeeT_debugger.h"

BEET_bool BeeT_Debugger_Init(BeeT_debugger * debugger, int port)
{
	debugger->initialized = BEET_TRUE;
	debugger->port = port;

	if (SDLNet_Init() < 0)
	{
		return BEET_FALSE;
	}
	IPaddress ip;
	if (SDLNet_ResolveHost(&ip, "localhost", port) == -1)
	{
		return BEET_FALSE;
	}

	TCPsocket tcpsock;
	tcpsock = SDLNet_TCP_Open(&ip);
	if (!tcpsock)
	{
		return BEET_FALSE;
	}
	return BEET_TRUE;
}
