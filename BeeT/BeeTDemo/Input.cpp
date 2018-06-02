#include "Input.h"
#include <string.h>

#include "SDL/include/SDL.h"

#define MAX_KEYS 300

Input::Input()
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
}

Input::~Input()
{
	delete keyboard;
}

bool Input::Update()
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	if (keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return false;
	if (keyboard[SDL_SCANCODE_F1] == KEY_UP)
		debug = !debug;
	SDL_GetMouseState(&mouseX, &mouseY);
	//screen size = 3
	mouseX /= 3;
	mouseY /= 3;
	return true;
}
