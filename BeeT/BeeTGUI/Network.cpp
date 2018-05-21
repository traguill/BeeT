#include "Network.h"
#include "Log.h"
#include "Application.h"
#include "Packet.h"

#include "../SDL2/include/SDL.h"

Network::Network(const char* name) : Module(name)
{}

Network::~Network()
{
}

bool Network::Init()
{
	if (SDLNet_Init() < 0)
	{
		LOGE("SDLNet init: %s", SDLNet_GetError());
		return false;
	}
	LOGI("SDLNet initialized");

	// Create a socket set
	socketSet = SDLNet_AllocSocketSet(maxSockets);
	if (socketSet == NULL)
	{
		LOGE("SDLNet create socket set: %s", SDLNet_GetError());
		return false;
	}

	// Initialize all the clients sockets
	maxClients = maxSockets - 1;
	clientSocket.resize(maxClients);
	std::fill(clientSocket.begin(), clientSocket.end(), nullptr);
	freeSockets.resize(maxClients);
	std::fill(freeSockets.begin(), freeSockets.end(), true);

	
	const unsigned short port = 8080;
	int hostResolved = SDLNet_ResolveHost(&serverIP, NULL, port);
	if (hostResolved == -1)
	{
		LOGE("SDLNet resolve host: %s", SDLNet_GetError());
		return false;
	}

	ListenConnections();
	g_app->AddModuleUpdate(this);
	return true;
}

bool Network::CleanUp()
{
	LOGI("SDLNet quitting");

	SDLNet_FreeSocketSet(socketSet);
	if (listening)
		SDLNet_TCP_Close(serverSocket);

	SDLNet_Quit();
	return true;
}

bool Network::Update()
{
	if (listening)
	{
		HandleNewConnections();
		HandleClientConnections();
	}
	return true;
}

bool Network::ListenConnections()
{
	if (!listening)
	{
		// Open the server socket
		serverSocket = SDLNet_TCP_Open(&serverIP);
		if (!serverSocket)
		{
			LOGE("SDLNet failed to open Server socket %s", SDLNet_GetError());
		}
		else
		{
			SDLNet_TCP_AddSocket(socketSet, serverSocket);
			listening = true;
		}
	}
	return listening;
}

void Network::StopListeningConnections()
{
	if (listening)
	{
		SDLNet_TCP_DelSocket(socketSet, serverSocket);
		SDLNet_TCP_Close(serverSocket);
		serverSocket = NULL;
		listening = false;
	}
}

void Network::HandleNewConnections()
{
	int numActiveSockets = SDLNet_CheckSockets(socketSet, 0);
	if (numActiveSockets == 0)
		return;
	int serverSocketReady = SDLNet_SocketReady(serverSocket);

	if (serverSocketReady != 0)
	{
		auto freeSpotIt = std::find(freeSockets.begin(), freeSockets.end(), true);
		*freeSpotIt = false;
		unsigned int index = freeSpotIt - freeSockets.begin();
		
		clientSocket[index] = SDLNet_TCP_Accept(serverSocket);
		
		SDLNet_TCP_AddSocket(socketSet, clientSocket[index]);
		numClientsConnected++;

		LOGI("New connection");
		
		Packet* packet = new Packet(PacketType::PT_CONNECTION_ACK, NULL, 0);
		int len = 0;
		char* data = packet->PrepareToSend(len);
		int ret = SDLNet_TCP_Send(clientSocket[index], data, len);
		if (ret < len)
		{
			LOGE("Error sending new connection acknowledgement: %s", SDLNet_GetError());
		}
		delete packet;
	}
	else // No space for new Clients
	{
		LOGW("Network: Maximum number of clients connected. Rejecting connection");

		TCPsocket tmpSocket = SDLNet_TCP_Accept(serverSocket); // Accept to clear it from incoming connections
		// Send msg refusing connection
		//SDLNet_TCP_Send(tmpSocket, (void*)buffer, length);

		SDLNet_TCP_Close(tmpSocket);
	}
}

void Network::HandleClientConnections()
{
	for (int id = 0; id < maxClients; id++)
	{
		int clientSocketReady = SDLNet_SocketReady(clientSocket[id]);
		if (clientSocketReady)
		{
			Packet* packet = Packet::Read(&clientSocket[id]);

			// Either the connection was closed or an error occurred
			if (packet == nullptr)
			{
				SDLNet_TCP_DelSocket(socketSet, clientSocket[id]);
				SDLNet_TCP_Close(clientSocket[id]);
				clientSocket[id] = nullptr;
				freeSockets[id] = true;
				numClientsConnected--;
			}
			else // Data has been read successfully
			{
				//TODO: Switch by packet type: Now only stuff
				// Send recv ack
				//SDLNet_TCP_Send(clientSocket[id], (void*)buffer, length);
			}
		}
	}
}
