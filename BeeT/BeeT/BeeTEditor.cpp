#include "BeeTEditor.h"
#include "BeeTGui.h"
#include "Log.h"
#include "Application.h"
#include "Window.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "BehaviorTree.h"
#include "BTNode.h"

#include <vector>

namespace ne = ax::NodeEditor;
using namespace std;

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
	editorSize.x = (float) screenWidth;
	editorSize.y = screenHeight - (ImGui::GetCursorPosY() - ImGui::GetCursorPosX());

	Editor();
	Inspector();
	return true;
}

bool BeeTEditor::CleanUp()
{
	delete bt;
	return true;
}

void BeeTEditor::Editor()
{
	
	ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetCursorPosY() - ImGui::GetCursorPosX())); // The Y component substracts the cursorX position because imgui by default has margins
	ImGui::SetNextWindowSize(ImVec2(editorSize.x * editorCanvasSize.x, editorSize.y * editorCanvasSize.y));
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("BeeT Editor Window", &beetEditorWindowOpen, flags);

	ne::SetCurrentEditor(g_app->beetGui->GetNodeEditorContext());
	ne::Begin("BeeT Node Editor");

	if (ne::ShowBackgroundContextMenu())
	{
		ImGui::OpenPopup("Create New Node");
	}
	
	// PopUps
	ShowPopUps();
	
	bt->Draw();
	ne::End(); // BeeT Node Editor
	ImGui::End();

	ImGui::PopStyleVar(); // WindowPadding
}

void BeeTEditor::Inspector()
{
	// Get the node selected at this frame
	if (ne::HasSelectionChanged())
	{
		if (ne::GetSelectedObjectCount() == 1)
		{
			ne::GetSelectedNodes(&selectedNodeId, 1);
		}
		else
			selectedNodeId = -1;
	}

	ImGui::SetNextWindowPos(ImVec2(screenWidth * 0.75f, ImGui::GetCursorPosY() - ImGui::GetCursorPosX()));
	ImGui::SetNextWindowSize(ImVec2(editorSize.x * inspectorSize.x, editorSize.y * inspectorSize.y));
	ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	
	if (selectedNodeId != -1)
	{
		BTNode* nodeSel = bt->FindNode(selectedNodeId);
		if (nodeSel)
		{
			ImGui::Text("Type: %s", nodeSel->GetTypeName().data());
			ImGui::Text("Name: %s", nodeSel->GetName().data());
			ImGui::Spacing();
			ImGui::Text("Comment: ");
			ImGui::Spacing();
			ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent et malesuada lorem. Nam posuere gravida ultricies. Vivamus id elementum odio. Maecenas sed sem nunc. Maecenas mattis ex mauris, eget tincidunt dolor aliquet in. Maecenas eget mauris posuere.");

			ImGui::Separator();

			ImGui::Text("Input: 0");
			ImGui::Text("Outputs: 0");
		}
	}

	ImGui::End();
}

void BeeTEditor::ShowPopUps()
{
	ne::Suspend();
	// Create a new node
	if (ImGui::BeginPopup("Create New Node"))
	{
		ImVec2 cursorPosition = ImGui::GetMousePosOnOpeningCurrentPopup();
		vector<NodeType> typesList = g_app->beetGui->btNodeTypes->GetTypeList();

		for (auto nodeType : typesList)
		{
			if (ImGui::MenuItem(nodeType.name.data()))
			{
				bt->AddNode(cursorPosition.x, cursorPosition.y, nodeType.typeId);
			}
		}
		ImGui::EndPopup();
	}
	ne::Resume();
}
