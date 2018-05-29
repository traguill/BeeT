#ifndef __PACKET_H__
#define __PACKET_H__

#include "../SDL2/include/SDL_net.h"
#include "../SharedData/SharedEnums.h"

class Packet
{
public:
	Packet();
	Packet(PacketType type, void* data, int size);
	~Packet();

	static Packet* Read(TCPsocket* socket);
	char* PrepareToSend(int& length);
	char* GetData(int& dataSize) const;
	PacketType GetType()const;

private:
	PacketType type;
	char* outData = nullptr; // Data to send
	char* inData = nullptr;  // Data received
	int totalSize = 0;

};
#endif // ! __PACKET_H__
