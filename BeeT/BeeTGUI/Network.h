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

	bool IsListening()const;

	void ApplySettings(); // Turns all temporary settings into permanent and applies them
	void ResetSettings();

private:

	void StopListeningConnections();
	bool StartListenngConnections();
	bool ListenConnections();

	void HandleNewConnections();
	void HandleClientConnections();

public:
	BeeTDebugger* beetDebugger = nullptr; // Pointer to the debugger. It is automatically set once the debugger is created

	// Settings
	bool settingsDirty = false;
	int tmpPort = 8080;
	std::string tmpIp = "127.0.0.1";
	bool tmpListening = false;

private:
	IPaddress serverIP;
	int port = 8080;
	std::string ip = "127.0.0.1";

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

