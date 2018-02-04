#include "Window.h"
#include "Log.h"

Window::Window(const char* name) : Module(name)
{
}

Window::~Window()
{
}

bool Window::Init()
{
	LOGI("SDL_Window initialization")
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOGE("SDL_VIDEO could not initialize.\nSDL_Error: %s", SDL_GetError());
		return false;
	}

	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	
	sdlWindow = SDL_CreateWindow("BeeT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, flags);
	if (sdlWindow == NULL)
	{
		LOGE("SDL Window could not be created. \nSDL_Error: %s", SDL_GetError());
		return false;
	}

	return true;
}

bool Window::CleanUp()
{
	LOGI("Destroying SDL Window and quitting SDL");
	if (sdlWindow)
	{
		SDL_DestroyWindow(sdlWindow);
	}
	SDL_Quit();
	return true;
}