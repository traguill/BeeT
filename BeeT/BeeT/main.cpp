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