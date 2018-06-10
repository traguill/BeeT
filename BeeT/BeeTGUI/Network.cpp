#include "Network.h"
#include "Log.h"
#include "Application.h"
#include "Packet.h"
#include "BeeTDebugger.h"
#include "BeeTGui.h"
#include "Random.h"

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

bool Network::IsListening() const
{
	return listening;
}

bool Network::ListenConnections()
{
	// Open the server socket
	serverSocket = SDLNet_TCP_Open(&serverIP);
	if (!serverSocket)
	{
		LOGE("SDLNet failed to open Server socket %s", SDLNet_GetError());
		return false;
	}
	else
	{
		SDLNet_TCP_AddSocket(socketSet, serverSocket);
	}
	return true;
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

bool Network::StartListenngConnections()
{
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

	
	int hostResolved = SDLNet_ResolveHost(&serverIP, ip.compare("127.0.0.1") ? ip.data() : NULL, port);
	if (hostResolved == -1)
	{
		LOGE("SDLNet resolve host: %s", SDLNet_GetError());
		return false;
	}

	bool ret = ListenConnections();
	if (ret)
	{
		LOGI("Server initialized successfully at: %s:%i", ip.data(), port);
	}
	return ret;
}

void Network::ApplySettings()
{
	port = tmpPort;
	ip = tmpIp;
	if (listening)
		StopListeningConnections();
	listening = tmpListening;
	if (listening)
		listening = StartListenngConnections();
	tmpListening = listening;
}

void Network::ResetSettings()
{
	tmpPort = port;
	tmpIp = ip;
	tmpListening = listening;
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

		int connectionUID = g_rnd->RandomInt(); // Sent a unique ID for this connection
		LOGI("New connection: %i", connectionUID);
		Packet* packet = new Packet(PacketType::PT_CONNECTION_ACK, &connectionUID, sizeof(int));
		int len = 0;
		char* data = packet->PrepareToSend(len);
		int ret = SDLNet_TCP_Send(clientSocket[index], data, len);
		if (ret < len)
		{
			LOGE("Error sending new connection acknowledgement: %s", SDLNet_GetError());
		}
		beetDebugger->OpenNewConnection(connectionUID);
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
				// Get data
				int recvDataSize = 0;
				char* recvData = packet->GetData(recvDataSize);

				Packet * ackPacket = nullptr;
				// Send acknowledgement
				switch (packet->GetType())
				{
					case PT_BT_FILE:
					ackPacket = new Packet(PacketType::PT_BT_FILE_ACK, NULL, 0);
					break;
					case PT_BT_UPDATE:
						ackPacket = new Packet(PacketType::PT_BT_UPDATE_ACK, NULL, 0);
						break;
				}
				if (ackPacket)
				{
					int len = 0;
					char* data = ackPacket->PrepareToSend(len);
					int ret = SDLNet_TCP_Send(clientSocket[id], data, len);
					if (ret < len)
					{
						LOGE("Error sending  acknowledgement: %s", SDLNet_GetError());
					}
					delete ackPacket;
				}
				// Process data
				beetDebugger->HandleIncomingData(recvData, recvDataSize, packet->GetType());
				delete packet;
			}
		}
	}
}
