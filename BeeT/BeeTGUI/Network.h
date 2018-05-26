#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "Module.h"
#include "../SDL2/include/SDL_net.h"
#include <vector>

class BeeTDebugger;

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

public:
	BeeTDebugger* beetDebugger = nullptr; // Pointer to the debugger. It is automatically set once the debugger is created

private:
	IPaddress serverIP;

	unsigned short maxSockets = 4;
	unsigned short maxClients;
	std::vector<TCPsocket> clientSocket;
	std::vector<bool> freeSockets;
	SDLNet_SocketSet socketSet;
	TCPsocket serverSocket;

	bool listening = false;
	int numClientsConnected = 0;

};
#endif // !__NETWORK_H__

