#ifndef __BEET_NW_PACKET_H__
#define __BEET_NW_PACKET_H__

#include "ThirdParty/SDL_net.h"
#include "beet_std.h"

#define MAX_PACKET_SIZE 512

typedef enum PacketType // Linked with BeeT.GUI Network.h enum
{
	PT_CONNECTION_ACK,
	PT_NULL_STATE
}PacketType;

typedef struct BeeT_packet BeeT_packet;
struct BeeT_packet
{
	PacketType	type;
	int			size;
	void*		data;	
};

void BeeT_packet_Cleanup(BeeT_packet* packet);

BEET_bool BeeT_packet_Read(BeeT_packet* packet, TCPsocket* socket);

BeeT_packet* BeeT_packet_Create(PacketType type, void* data, int size);

#endif // !__BEET_NW_PACKET_H__
