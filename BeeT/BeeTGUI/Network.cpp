#include "Network.h"
#include "Log.h"
#include "Application.h"

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

	msgBuffer = new char[MESSAGE_MAX_SIZE];
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

	if (msgBuffer)
		delete[] msgBuffer;
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
		
		//Send Acknowledgement for connection
		int msg = 1;
		int len = sizeof(msg);
		int ret = SDLNet_TCP_Send(clientSocket[index], &msg, len);
		if (ret < len)
		{
			LOGE("Error sending new connection acknowledgement: %s", SDLNet_GetError());
		}
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
		if (clientSocketReady != 0)
		{

			int dataRecv = SDLNet_TCP_Recv(clientSocket[id], msgBuffer, MESSAGE_MAX_SIZE);

			// Either the connection was closed or an error occurred
			if (dataRecv <= 0)
			{
				SDLNet_TCP_DelSocket(socketSet, clientSocket[id]);
				SDLNet_TCP_Close(clientSocket[id]);
				clientSocket[id] = nullptr;
				freeSockets[id] = true;
				numClientsConnected--;
			}
			else // Data has been read successfully
			{
				// Send recv ack
				//SDLNet_TCP_Send(clientSocket[id], (void*)buffer, length);
			}
		}
	}
}
