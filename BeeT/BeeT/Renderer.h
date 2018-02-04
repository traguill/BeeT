#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Module.h"
#include "External/SDL2-2.0.7/include/SDL.h"

class Renderer : public Module
{
public:
	Renderer(const char* name);
	~Renderer();

	bool Init();
	bool CleanUp();

	bool PreUpdate();
	bool PostUpdate();

private:
	bool CheckGLError()const;

private:
	SDL_GLContext context;
};
#endif
