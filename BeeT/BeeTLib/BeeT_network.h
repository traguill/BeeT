#ifndef __BEET_NETWORK_H__
#define __BEET_NETWORK_H__

#include "ThirdParty/SDL_net.h"
#include "beet_std.h"

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

// Network-------------------------------------------------

typedef struct BeeT_network BeeT_network;
struct BeeT_network
{
	int port;
	IPaddress ip;

	SDLNet_SocketSet socketSet;
	dequeue* socketList;
};

BeeT_network*	BeeT_NW_Init(int port);
void			BeeT_NW_Tick(BeeT_network* nw);


#endif // !__BEET_NETWORK_H__

