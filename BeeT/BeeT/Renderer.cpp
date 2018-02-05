#include "Renderer.h"
#include "Application.h"
#include "Window.h"
#include "Log.h"
#include "External/glew-2.1.0/include/glew.h"
#include "External/SDL2-2.0.7/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "External/glew-2.1.0/lib/x86/glew32.lib")

#include "External/ImGui/imgui.h"
#include "External/ImGui/imgui_impl_sdl_gl3.h"

Renderer::Renderer(const char* name) : Module(name)
{}

Renderer::~Renderer()
{}

bool Renderer::Init()
{
	LOGI("Creating SDL OpenGL renderer context");
	context = SDL_GL_CreateContext(g_app->window->sdlWindow);
	if (context == NULL)
	{
		LOGE("OpenGL context could not be created.\nSDL_Error: %s", SDL_GetError());
		return false;
	}
	GLenum glError = glewInit();
	if (glError != GLEW_OK)
	{
		LOGE("Glew failed to init.\nGlewError: %s", glewGetErrorString(glError));
		return false;
	}
	LOGI("Using Glew %s version", glewGetString(GLEW_VERSION));
	LOGI("Using OpenGL %s version", glGetString(GL_VERSION));
	LOGI("Vendor: %s", glGetString(GL_VENDOR));
	LOGI("Renderer: %s", glGetString(GL_RENDERER));
	LOGI("Shading language version: GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		LOGW("Unable to set VSync.\nSDL_Error: %s", SDL_GetError());
	}
	else
		LOGI("VSync enabled");

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (CheckGLError() == false)
		return false;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (CheckGLError() == false)
		return false;

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	if (CheckGLError() == false)
		return false;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLfloat lightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);

	GLfloat materialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);

	GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	// ImGui init
	LOGI("Init ImGui");
	ImGui_ImplSdlGL3_Init(g_app->window->sdlWindow);

	// Add PreUpdate and PostUpdate to App steps
	g_app->AddModulePreUpdate(this);
	g_app->AddModulePostUpdate(this);
	
	return true;
}

bool Renderer::CleanUp()
{
	LOGI("Shutting down ImGui");
	ImGui_ImplSdlGL3_Shutdown();
	LOGI("Destroying SDL OpenGL renderer context");
	SDL_GL_DeleteContext(context);
	return true;
}

bool Renderer::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplSdlGL3_NewFrame(g_app->window->sdlWindow);
	return true;
}

bool Renderer::PostUpdate()
{
	ImGui::Render();
	SDL_GL_SwapWindow(g_app->window->sdlWindow);
	return true;
}

bool Renderer::CheckGLError() const
{
	GLenum glError = glGetError();
	if (glError != GL_NO_ERROR)
	{
		LOGE("An error has occured initializing OpenGL.\n%s", gluErrorString(glError));
		return false;
	}
	return true;
}