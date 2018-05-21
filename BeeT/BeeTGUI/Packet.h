#ifndef __PACKET_H__
#define __PACKET_H__

#include "../SDL2/include/SDL_net.h"

enum PacketType // Linked with BeeT.Lib BeeT_NW_packet.h enum
{
	PT_CONNECTION_ACK,
	PT_NULL_STATE
};

class Packet
{
public:
	Packet();
	Packet(PacketType type, void* data, int size);
	~Packet();

	static Packet* Read(TCPsocket* socket);
	char* PrepareToSend(int& length);

private:
	PacketType type;
	char* outData = nullptr; // Data to send
	char* inData = nullptr;  // Data received
	int totalSize = 0;

};
#endif // ! __PACKET_H__
