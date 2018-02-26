#include "Input.h"
#include "Application.h"

#include "../SDL2/include/SDL.h"
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_impl_sdl_gl3.h"

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
