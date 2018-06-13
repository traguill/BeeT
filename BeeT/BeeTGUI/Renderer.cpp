#include "Renderer.h"
#include "Application.h"
#include "Window.h"
#include "Log.h"
#include "../Glew/include/GL/glew.h"
#include "../SDL2/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_impl_sdl_gl3.h"

#include "IconsFont.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#ifdef _DEBUG
	#pragma comment( lib, "../lib/glew_static_x86d.lib" )
#else
	#pragma comment( lib, "../lib/glew_static_x86.lib" )
#endif


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
	ImGui::GetIO().IniFilename = NULL; // Avoids creating the imgui.ini file

	// Init fonts
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("RobotoCondensed-Bold.ttf", 16);
	static const ImWchar icons_ranges[] = { 0xe000, 0xf3ff, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.MergeGlyphCenterV = true;
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAR, 16.0f, &icons_config, icons_ranges);

	InitImGuiStyle(); // Set new style

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

void Renderer::InitImGuiStyle()
{
	ImGuiStyle * style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 0.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.65f, 0.00f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.08f, 0.08f, 0.08f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 0.72f, 0.16f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.72f, 0.16f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(1.00f, 0.75f, 0.26f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.71f, 0.71f, 0.71f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	style->Colors[ImGuiCol_ComboBg] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.37f, 0.62f, 1.00f, 0.78f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.31f, 0.31f, 0.31f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.16f, 0.50f, 1.00f, 0.49f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.16f, 0.50f, 1.00f, 0.76f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.29f, 0.70f, 0.75f);
	style->Colors[ImGuiCol_Header] = ImVec4(1.00f, 0.65f, 0.00f, 0.73f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.90f, 0.59f, 0.14f, 0.47f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.85f, 0.38f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.00f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CloseButton] = ImVec4(1.00f, 1.00f, 1.00f, 0.46f);
	style->Colors[ImGuiCol_CloseButtonHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.75f);
	style->Colors[ImGuiCol_CloseButtonActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.38f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.91f, 0.66f, 0.05f, 0.56f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 1.00f, 0.95f, 0.73f);


}
