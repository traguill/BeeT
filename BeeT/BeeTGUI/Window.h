#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"
#include "../SDL2/include/SDL.h"

class Window : public Module
{
public:
	Window(const char* name);
	~Window();

	bool Init();
	bool CleanUp();

	void OnResize(int width, int height);
	void GetWindowSize(int& width, int& height)const;

public:
	SDL_Window* sdlWindow = nullptr;

private:

	// Desktop
	int screenWidth = 1270;
	int screenHeight = 960;
	
	// Laptop 
	//int screenWidth = 800;
	//int screenHeight = 600;
};
#endif