#ifndef __BEET_NETWORK_H__
#define __BEET_NETWORK_H__

#include "ThirdParty/SDL_net.h"
#include "beet_std.h"

typedef enum SocketState
{
	NOT_INIT,
	OPEN,			// Waiting to be open
	WAITING_CONNECTION_ACK, // Waiting connection acknowledgement with the server
	READY_TO_SEND,	// Ready to send data
	WAITING_SEND_ACK,
	CLEANUP
}SocketState;

typedef struct BeeT_socket BeeT_socket;
struct BeeT_socket
{
	TCPsocket socket;
	SocketState state;
	struct BeeT_DBG_BT* bt;
	unsigned int sentDataSize;
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

BEET_bool		BeeT_NW_OpenSocket(BeeT_network* nw, struct BeeT_DBG_BT* bt);
BEET_bool		BeeT_NW_SocketReadyToSend(BeeT_socket* sc); // Returns true if any data was sent
BEET_bool		BeeT_NW_SocketWaitingSendAck(BeeT_socket* sc); // Returns true if ack was received


#endif // !__BEET_NETWORK_H__

