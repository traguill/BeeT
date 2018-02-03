#include "Application.h"
#include "Log.h"

#include "External/SDL2-2.0.7/include/SDL.h"
#pragma comment( lib, "External/SDL2-2.0.7/lib/x86/SDL2.lib" )
#pragma comment( lib, "External/SDL2-2.0.7/lib/x86/SDL2main.lib" )

#define WIN32_MEAN_AND_LEAN

// Globals
Application* g_app = nullptr;

int main(int argc, char* argv[])
{
	bool ret = false;

	g_app = new Application();	
	ret = g_app->Init();
	if (ret == false)
	{
		LOGE("Initialization failed");
	}

	// Loop

	ret = g_app->CleanUp();
	if (ret == false)
	{
		LOGE("One or more modules failed at CleanUp. See errors above.");
	}

	delete g_app;

	return 0;
}