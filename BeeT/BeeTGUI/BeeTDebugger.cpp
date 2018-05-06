#include "BeeTDebugger.h"
#include "Application.h"
#include "Window.h"
#include "ThirdParty/ImGui/imgui.h"

BeeTDebugger::BeeTDebugger()
{
}

BeeTDebugger::~BeeTDebugger()
{
}

void BeeTDebugger::Init()
{
}

bool BeeTDebugger::Update()
{
	int screenWidth, screenHeight;
	g_app->window->GetWindowSize(screenWidth, screenHeight);

	screenHeight -= (ImGui::GetCursorPosY() - ImGui::GetCursorPosX());

	ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetCursorPosY() - ImGui::GetCursorPosX()));
	ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight));
	ImGui::Begin("Debugger", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	
	ImGui::Button("Start###debugger_start");
	
	ImGui::End();
	
	return true;
}

void BeeTDebugger::CleanUp()
{
}
