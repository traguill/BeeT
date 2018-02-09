#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"
#include "ThirdParty/SDL2-2.0.7/include/SDL.h"

class Window : public Module
{
public:
	Window(const char* name);
	~Window();

	bool Init();
	bool CleanUp();

	void GetWindowSize(int& width, int& height)const;

public:
	SDL_Window* sdlWindow = nullptr;

private:
	int screenWidth = 1270;
	int screenHeight = 860;
};
#endif