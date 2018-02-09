#include "BeeTEditor.h"
#include "BeeTGui.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Log.h"
#include "Application.h"
#include "Window.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"

namespace ne = ax::NodeEditor;

BeeTEditor::BeeTEditor()
{
}

BeeTEditor::~BeeTEditor()
{
}

bool BeeTEditor::Update()
{
	// This part should be inside the Init()
	bool beetEditorWindowOpen = true;
	ImGuiWindowFlags flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
							|ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar
							|ImGuiWindowFlags_::ImGuiWindowFlags_NoMove;
	int screenWidth = 0;
	int screenHeight = 0;
	g_app->window->GetWindowSize(screenWidth, screenHeight);
	ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetCursorPosY() - ImGui::GetCursorPosX())); // The Y component substracts the cursorX position because imgui by default has margins
	ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight - (ImGui::GetCursorPosY() - ImGui::GetCursorPosX())));
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	ImGui::Begin("BeeT Editor Window", &beetEditorWindowOpen, flags);
	
	ne::SetCurrentEditor(g_app->beetGui->GetNodeEditorContext());
	ne::Begin("BeeT Node Editor");
	ne::End(); // BeeT Node Editor

	ImGui::End();
	return true;
}
