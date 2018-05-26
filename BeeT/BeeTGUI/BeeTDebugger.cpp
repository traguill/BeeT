#include "BeeTDebugger.h"
#include "Application.h"
#include "Window.h"
#include "Network.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Log.h"

BeeTDebugger::BeeTDebugger()
{
}

BeeTDebugger::~BeeTDebugger()
{
}

void BeeTDebugger::Init()
{
	g_app->network->beetDebugger = this;
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

void BeeTDebugger::HandleIncomingData(char * buf, int size, PacketType type)
{
	switch (type)
	{
	case PT_BT_FILE:
		LOGI("BT Received: \n %s", buf);
		break;
	}
}