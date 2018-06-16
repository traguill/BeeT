#include "../BeeTLib/beet.h"

#ifdef _DEBUG
	#pragma comment(lib, "../build/demo/BeeTLibd.lib")
#else
	#pragma comment(lib, "../build/demo/BeeTLib.lib")
#endif // DEBUG

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_timer.h"
#include "Input.h"
#include "GameManager.h"
#include "Physics.h"

#include "Globals.h"

Input* g_Input = NULL;
GameManager* g_GameManager = NULL;
Physics * g_Physics = NULL;
Uint64 frequency, timeStart;

float LastFrameSec();

NodeStatus TaskCallbackFunc(unsigned int btId, const char* taskId, NodeFunction func);

int main(int argc, char* args[])
{
	BEET_Init();
	BEET_SetTaskCallbackFunc(TaskCallbackFunc);
	BEET_InitDebugger("127.0.0.1", 8080);
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

	// Create Input
	g_Input = new Input();
	// Create Physics
	g_Physics = new Physics();
	g_Physics->renderer = renderer;
	// Create Game Manager
	g_GameManager = new GameManager(renderer);
	g_GameManager->Init();

	// Background
	SDL_Texture* bg = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("Game/background.bmp"));
	SDL_Rect bgRect = { 0, 0, 900, 640 };

	
	// Timer
	frequency = SDL_GetPerformanceFrequency();
	timeStart = SDL_GetPerformanceCounter();
	
	float time = 0.0f;
	// Loop
	while (g_Input->Update())
	{
		// Logic ------------------
		float dt = LastFrameSec();
		if (dt > 0.1)
			dt = 0.00016;
		BEET_Tick(dt);
		g_Physics->Tick();
		g_GameManager->Tick(dt);

		time += dt;
		char buffer[256];
		size_t siz = sprintf_s(buffer, "Time: %.2f", time);
		SDL_SetWindowTitle(window, buffer);
		// Graphics ---------------

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
	
		SDL_RenderCopy(renderer, bg, NULL, &bgRect);
	
		g_GameManager->Draw();
		if(g_Input->debug)
			g_Physics->DrawColliders();
		SDL_RenderPresent(renderer);
	}

	delete g_GameManager;
	delete g_Physics;
	delete g_Input;
	BEET_Shutdown();
	SDL_Quit();
	return(0);
}

float LastFrameSec()
{
	double ret = (double(SDL_GetPerformanceCounter() - timeStart) / double(frequency));
	timeStart = SDL_GetPerformanceCounter();
	return ret;
}

NodeStatus TaskCallbackFunc(unsigned int btId, const char* taskId, NodeFunction func)
{
	NodeStatus ret = NS_INVALID;
	switch (func)
	{
	case NF_ONINIT:
		g_GameManager->OnInitBTTask(btId, taskId);
		break;
	case NF_UPDATE:
		ret = g_GameManager->UpdateBTTask(btId, taskId);
		break;
	case NF_ONFINISH:
		g_GameManager->OnFinishBTTask(btId, taskId);
		break;
	}
	return ret;
}