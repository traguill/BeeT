#ifndef __BEET_DEBUGGER_H__
#define __BEET_DEBUGGER_H__

#include "ThirdParty/SDL_net.h"
#include "beet_std.h"

typedef struct BeeT_debugger BeeT_debugger;
struct BeeT_debugger
{
	BEET_bool initialized;
	BEET_bool enabled;
	int port;
	IPaddress ip;

	SDLNet_SocketSet socketSet;
	dequeue* socketList;
};

BEET_bool BeeT_Debugger_Init(BeeT_debugger* debugger, int port);
void BeeT_Debugger_Tick(BeeT_debugger* debugger);

// ----------------------------------------------------------------
// Network
// ----------------------------------------------------------------
typedef enum SocketState
{
	NOT_INIT,
	OPEN,			// Waiting to be open
	CONNECTION_ACK, // Waiting connection acknowledgement with the server
	READY_TO_SEND,	// Ready to send data
	SEND_ACK,
	CLEANUP
}SocketState;

typedef struct BeeT_socket BeeT_socket;
struct BeeT_socket
{
	TCPsocket socket;
	SocketState state;
};

#endif // !__BEET_DEBUGGER_H__
