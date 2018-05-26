#include "BeeT_NW_packet.h"

#define INT_SIZE sizeof(int)

void BeeT_packet_Cleanup(BeeT_packet * packet)
{
	BEET_ASSERT(packet != NULL);
	if (packet->data)
		BEET_free(packet->data);
	BEET_free(packet);
}

BEET_bool BeeT_packet_Read(BeeT_packet * packet, TCPsocket * socket)
{
	char* tmp = (char*)BEET_malloc(MAX_PACKET_SIZE);

	BEET_bool initialized = BEET_FALSE;
	int packCount = 0;
	int numPackets = 0;
	int offset = 0;
	char* pointer = NULL;
	int dataRecv;
	do
	{
		dataRecv = SDLNet_TCP_Recv(*socket, tmp, MAX_PACKET_SIZE);
		if (dataRecv > 0)
		{
			if (!initialized)
			{
				BEET_memcpy(&numPackets, tmp, INT_SIZE);
				BEET_memcpy(&packet->type, tmp + INT_SIZE, INT_SIZE);
				BEET_memcpy(&offset, tmp + (INT_SIZE * 2), INT_SIZE);

				packet->data = BEET_malloc((numPackets * MAX_PACKET_SIZE) - offset);
				pointer = packet->data;
				packet->size = (numPackets * MAX_PACKET_SIZE) - offset;
				initialized = BEET_TRUE;
			}
			packCount++;

			if(packCount < numPackets)
			{
				BEET_memcpy(pointer, tmp, MAX_PACKET_SIZE);
				pointer += MAX_PACKET_SIZE;
			}
			else
			{
				BEET_memcpy(pointer, tmp, MAX_PACKET_SIZE - offset); // Last packet. Do not copy the offset bytes
			}
		}
	} while (packCount < numPackets && dataRecv > 0);

	if (tmp)
		BEET_free(tmp);
	
	return dataRecv > 0 ? BEET_TRUE : BEET_FALSE;
}

BeeT_packet * BeeT_packet_Create(PacketType type, void * data, int size)
{
	BeeT_packet* packet = (BeeT_packet*)BEET_malloc(sizeof(BeeT_packet));
	packet->type = type;

	// Header: NumPackets + Type + Offset = sizeof(int * 3)
	int totalDataSize = sizeof(int) * 3 + size;
	int offset = (totalDataSize < MAX_PACKET_SIZE) ? MAX_PACKET_SIZE - totalDataSize : MAX_PACKET_SIZE  - (totalDataSize % MAX_PACKET_SIZE);
	packet->size = totalDataSize + offset;
	int numPackets = packet->size / MAX_PACKET_SIZE;

	// Fill data
	packet->data = BEET_malloc(packet->size);
	char* pointer = (char*)packet->data;

	int intSize = sizeof(int);
	BEET_memcpy(pointer, &numPackets, intSize);
	pointer += intSize;
	BEET_memcpy(pointer, &packet->type, intSize);
	pointer += intSize;
	BEET_memcpy(pointer, &offset, intSize);
	pointer += intSize;

	if(data != NULL)
		BEET_memcpy(pointer, data, size);

	return packet;
}
