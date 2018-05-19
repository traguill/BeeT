#include "BeeT_NW_packet.h"

void BeeT_packet_Cleanup(BeeT_packet * packet)
{
	if (packet->allData)
		BEET_free(packet->allData);
}

BEET_bool BeeT_packet_Read(BeeT_packet * packet, TCPsocket * socket)
{
	packet->allData = BEET_malloc(MAX_PACKET_SIZE);

	int dataRecv = SDLNet_TCP_Recv(*socket, packet->allData, MAX_PACKET_SIZE);

	if (dataRecv > 0)
	{
		size_t typeSize = sizeof(packet->type);
		BEET_memcpy(&packet->type, packet->allData, typeSize);

		packet->data = (void*)((char*)packet->allData + typeSize);
	}
	
	
	return dataRecv > 0 ? BEET_TRUE : BEET_FALSE;
}
