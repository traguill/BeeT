#include "BeeT_network.h"
#include "BeeT_NW_packet.h"

#define BEET_DB_MAX_NUM_SOCKETS 3 //TODO: Change this

BeeT_network * BeeT_NW_Init(int port)
{
	BeeT_network* nw = (BeeT_network*)BEET_malloc(sizeof(BeeT_network));
	nw->port = port;

	if (SDLNet_Init() < 0)
	{
		printf("%s\n", SDLNet_GetError());
		return NULL;
	}
	if (SDLNet_ResolveHost(&nw->ip, "localhost", port) == -1)
	{
		printf("%s\n", SDLNet_GetError());
		return NULL;
	}

	nw->socketSet = SDLNet_AllocSocketSet(BEET_DB_MAX_NUM_SOCKETS);
	if (nw->socketSet == NULL)
	{
		printf("SDLNet create socket set: %s\n", SDLNet_GetError());
		return NULL;
	}

	nw->socketList = InitDequeue();

	for (int i = 0; i < BEET_DB_MAX_NUM_SOCKETS; i++)
	{
		BeeT_socket* sc = (BeeT_socket*)BEET_malloc(sizeof(BeeT_socket));
		sc->state = OPEN;
		sc->socket = NULL;
		dequeue_push_back(nw->socketList, sc);
	}

	return nw;
}

void BeeT_NW_Tick(BeeT_network* nw)
{
	SDLNet_CheckSockets(nw->socketSet, 0);
	node_deq* it = dequeue_head(nw->socketList);
	while (it != NULL)
	{
		BeeT_socket* sc = (BeeT_socket*)it->data;

		if (sc->state == NOT_INIT)
		{
			it = it->next;
			continue;
		}

		if (sc->state == OPEN)
		{
			TCPsocket tcpsock = SDLNet_TCP_Open(&nw->ip);
			if (tcpsock == NULL)
			{
				// PRINT ERROR
			}
			sc->socket = tcpsock;
			SDLNet_TCP_AddSocket(nw->socketSet, sc->socket);
			sc->state++;
		}

		if (sc->state == CONNECTION_ACK)
		{
			if (SDLNet_SocketReady(sc->socket))
			{
				BeeT_packet* packet = (BeeT_packet*)BEET_malloc(sizeof(BeeT_packet));
				BEET_bool readRet = BeeT_packet_Read(packet, &sc->socket);
				if (readRet)
				{
					if (packet->type == PT_CONNECTION_ACK)
					{
						printf("Connection established with Server\n");
						sc->state++;
					}
				}
				else
				{
					sc->state = CLEANUP;// Close connection
				}
				BeeT_packet_Cleanup(packet);
				BEET_free(packet);
			}
		}

		if (sc->state == CLEANUP)
		{
			SDLNet_TCP_DelSocket(nw->socketSet, sc->socket);
			SDLNet_TCP_Close(sc->socket);
			sc->state = NOT_INIT;
		}

		it = it->next;
	}
}
