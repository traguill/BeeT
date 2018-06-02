#include "../BeeTLib/beet.h"

#ifdef _DEBUG
	#pragma comment(lib, "../build/demo/BeeTLibd.lib")
#else
	#pragma comment(lib, "../build/demo/BeeTLib.lib")
#endif // DEBUG

#include "SDL/include/SDL.h"
#include "Input.h"
#include "Player.h"

Input* g_Input = NULL;

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 640

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window;
	SDL_Renderer* renderer;

	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

	// Create Input
	g_Input = new Input();

	// Background
	SDL_Texture* bg = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP("Game/background.bmp"));
	SDL_Rect bgRect = { 0, 0, 900, 640 };
	Player e(0.0f, 0.0f);
	e.LoadSprite(renderer, "Game/bee.bmp", 154, 220);
	e.speed = 0.1f;
	e.angle = 90;

	while (g_Input->Update())
	{
		// Logic ------------------

		e.Tick(1.0f);

		// Graphics ---------------

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, bg, NULL, &bgRect);
		e.Draw(renderer);

		SDL_RenderPresent(renderer);
	}

	delete g_Input;

	SDL_Quit();
	return(0);
}