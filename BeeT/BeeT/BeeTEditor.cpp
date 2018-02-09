#include "BeeTEditor.h"
#include "BeeTGui.h"
#include "Log.h"
#include "Application.h"
#include "Window.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "BehaviorTree.h"

namespace ne = ax::NodeEditor;

BeeTEditor::BeeTEditor()
{
}

BeeTEditor::~BeeTEditor()
{
}

bool BeeTEditor::Init()
{
	bt = new BehaviorTree();
	return true;
}

bool BeeTEditor::Update()
{
	g_app->window->GetWindowSize(screenWidth, screenHeight);
	ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetCursorPosY() - ImGui::GetCursorPosX())); // The Y component substracts the cursorX position because imgui by default has margins
	ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight - (ImGui::GetCursorPosY() - ImGui::GetCursorPosX())));

	ImGui::Begin("BeeT Editor Window", &beetEditorWindowOpen, flags);
	
	ne::SetCurrentEditor(g_app->beetGui->GetNodeEditorContext());
	ne::Begin("BeeT Node Editor");

	Editor();

	ne::End(); // BeeT Node Editor
	ImGui::End();
	return true;
}

bool BeeTEditor::CleanUp()
{
	delete bt;
	return true;
}

void BeeTEditor::Editor()
{
	ne::Suspend();
	
	// Input
	if (ImGui::IsMouseReleased(1))
	{
		HandleRightMouseButton();
	}

	// PopUps
	ShowPopUps();
	
	ne::Resume();

	bt->Draw();
}

void BeeTEditor::HandleRightMouseButton()
{
	if (ImGui::IsMouseHoveringWindow()) // TODO: Check if mouse is over the grid and not over a node or a window
	{
		ImGui::OpenPopup("Create New Node");
	}
}

void BeeTEditor::ShowPopUps()
{
	// Create a new node
	if (ImGui::BeginPopup("Create New Node"))
	{
		if (ImGui::MenuItem("Test Node #1"))
		{
			bt->AddNode(); // TODO: Pass the node type
		}
		if (ImGui::MenuItem("Test Node #2"))
		{

		}
		if (ImGui::MenuItem("Test Node #3"))
		{

		}
		ImGui::EndPopup();
	}
}
