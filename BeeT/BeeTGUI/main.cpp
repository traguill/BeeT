#include "Application.h"
#include "Log.h"

#include "../SDL2/include/SDL.h"

#include "../SDL2/src/main/windows/SDL_windows_main.c"

#ifdef _DEBUG
	#pragma comment( lib, "../lib/sdl2_static_x86d.lib" )
#else
	#pragma comment( lib, "../lib/sdl2_static_x86.lib" )
#endif

#define WIN32_MEAN_AND_LEAN

// Globals
Application* g_app = nullptr;

int SDL_main(int argc, char* argv[])
{
	bool ret = false;
	
	g_app = new Application();
	AppStep step = APP_START;

	while (step != AppStep::APP_QUIT)
	{
		switch (step)
		{
		case APP_START:
			ret = g_app->Init();
			if (ret == false)
			{
				LOGE("Initialization failed.");
				step = APP_QUIT;
			}
			else
			{
				LOGI("Initialization completed.");
				step = APP_LOOP;
			}
			break;
		case APP_LOOP:
			ret = g_app->Update();
			if (ret == false)
			{
				LOGI("Application starts to quit.");
				step = APP_END;
			}
			break;
		case APP_END:
			ret = g_app->CleanUp();
			if (ret == false)
			{
				LOGE("One or more modules failed at CleanUp. See errors above.");
			}
			else
			{
				LOGI("CleanUp completed.");
			}
			step = APP_QUIT;
			break;
		}
	}

	LOGI("Application quitting");
	delete g_app;

	return 0;
}