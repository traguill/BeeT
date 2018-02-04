#include "Input.h"
#include "Application.h"

#include "External/SDL2-2.0.7/include/SDL.h"
#include "External/imgui-1.53/imgui.h"
#include "External/imgui-1.53/imgui_impl_sdl_gl3.h"

Input::Input(const char* name) : Module(name)
{}

Input::~Input()
{}

bool Input::Init()
{
	g_app->AddModulePreUpdate(this);
	return true;
}

bool Input::PreUpdate()
{
	bool ret = true;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSdlGL3_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			ret = false;
	}
	return ret;
}
