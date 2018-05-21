#include "Packet.h"
#include <string.h>

#define MESSAGE_MAX_SIZE 512 // TODO: Change this

Packet::Packet()
{
}

Packet::Packet(PacketType type, void * data, int size) : type(type)
{
	totalSize = sizeof(int) * 3 + size;
	int offset = (totalSize < MESSAGE_MAX_SIZE) ? MESSAGE_MAX_SIZE - totalSize : totalSize % MESSAGE_MAX_SIZE;
	totalSize += offset;
	int numPackets = totalSize / MESSAGE_MAX_SIZE;
	outData = new char[totalSize];

	char* pointer = outData;
	// Header: numPackets + type + offset
	memcpy(pointer, &numPackets, sizeof(numPackets));
	pointer += sizeof(numPackets);
	memcpy(pointer, &type, sizeof(type));
	pointer += sizeof(type);
	memcpy(pointer, &offset, sizeof(offset));
	pointer += sizeof(offset);
	// Data
	if(data)
		memcpy(pointer, data, size);
}

Packet::~Packet()
{
	if (outData)
		delete[] outData;
}

Packet * Packet::Read(TCPsocket * socket)
{
	Packet* packet = nullptr;
	bool initialized = false;
	char* tmp = new char[MESSAGE_MAX_SIZE];
	int packCount = 0;
	int numPackets = 0;
	int offset = 0;
	char* pointer = nullptr;
	int dataRecv;
	do
	{
		dataRecv = SDLNet_TCP_Recv(*socket, tmp, MESSAGE_MAX_SIZE);
		if (dataRecv > 0)
		{
			if (!initialized)
			{
				packet = new Packet();
				memcpy(&numPackets, tmp, sizeof(int));
				memcpy(&packet->type, tmp + sizeof(int), sizeof(int));
				memcpy(&offset, tmp + (sizeof(int) * 2), sizeof(int));
				packet->inData = new char[(numPackets * MESSAGE_MAX_SIZE) - offset];
				pointer = packet->inData;
				initialized = true;
			}
			packCount++;

			if (packCount < numPackets)
			{
				memcpy(pointer, tmp, MESSAGE_MAX_SIZE);
				pointer += MESSAGE_MAX_SIZE;
			}
			else
			{
				memcpy(pointer, tmp, MESSAGE_MAX_SIZE - offset); // Last packet. Do not copy the offset data
			}
		}
	} while (packCount < numPackets && dataRecv > 0);

	if (tmp)
		delete[] tmp;

	return packet;
}

char * Packet::PrepareToSend(int & length)
{
	length = totalSize;
	return outData;
}
