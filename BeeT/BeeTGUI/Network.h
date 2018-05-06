#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "Module.h"
#include "../SDL2/include/SDL_net.h"
#include <vector>

#define MESSAGE_MAX_SIZE 512 // TODO: Change this

class Network : public Module
{
public:
	Network(const char* name);
	~Network();

	bool Init();
	bool CleanUp();
	bool Update();

	bool ListenConnections();
	void StopListeningConnections();

private:

	void HandleNewConnections();
	void HandleClientConnections();

private:
	IPaddress serverIP;

	unsigned short maxSockets = 4;
	unsigned short maxClients;
	std::vector<TCPsocket> clientSocket;
	std::vector<bool> freeSockets;
	SDLNet_SocketSet socketSet;
	TCPsocket serverSocket;

	bool listening = true;
	int numClientsConnected = 0;

	char* msgBuffer = nullptr; // Buffer to read incomming messages

};
#endif // !__NETWORK_H__

