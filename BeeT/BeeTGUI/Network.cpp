#include "Network.h"
#include "Log.h"

#include "../SDL2/include/SDL.h"
#include "../SDL2/include/SDL_net.h"

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
	return true;
}

bool Network::CleanUp()
{
	LOGI("SDLNet quitting");
	SDLNet_Quit();
	return true;
}
