#include "BeeTEditor.h"
#include "BeeTGui.h"
#include "Log.h"
#include "Application.h"
#include "Window.h"
#include "FileSystem.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "BehaviorTree.h"
#include "BTNode.h"
#include "BTLink.h"

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

void BeeTEditor::Serialize(const char* filename) const
{
	// In progress: Now only save one BT. In the future choose one of the opened BTs to save or save them all.
	char* buffer = nullptr;
	int size = bt->Serialize(&buffer);
	if (size == 0)
	{
		LOGE("Behavior Tree was not saved. An error occurred during serialization.");
	}
	else
	{
		unsigned int ret = g_app->fileSystem->Save(filename, buffer, size);
		if(ret == 0)
			LOGE("Behavior Tree was not saved. An error occurred while writting data on a file.")
	}

	if (buffer)
		delete buffer;
}

void BeeTEditor::Load(const char * path)
{
	char* buffer = nullptr;
	unsigned int size = g_app->fileSystem->Load(path, &buffer);
	if (size > 0)
	{
		Data btData(buffer);

		NewBehaviorTree(&btData); // For now the current BT is destroyed and replaced.
	}
	else
	{
		LOGE("Could not open the file %s", path);
	}

	if (buffer)
		delete buffer;
}

void BeeTEditor::NewBehaviorTree(Data* data)
{
	g_app->beetGui->ResetNodeEditorContext();
	if (bt)
		delete bt;
	if (data)
		bt = new BehaviorTree(*data);
	else
		bt = new BehaviorTree();
}

void BeeTEditor::Editor()
{
	
	ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetCursorPosY() - ImGui::GetCursorPosX())); // The Y component substracts the cursorX position because imgui by default has margins
	ImGui::SetNextWindowSize(ImVec2(editorSize.x * editorCanvasSize.x, editorSize.y * editorCanvasSize.y));
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("BeeT Editor Window", &beetEditorWindowOpen, flags);

	ne::Begin("BeeT Node Editor");

	Menus();

	bt->Draw();
	
	Links();
	// PopUps
	ShowPopUps();
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
			ImGui::Text("Type: %s", nodeSel->type->name.data());
			
			ImGui::Text("Name: "); ImGui::SameLine();
			char nodeNameTmp[_MAX_PATH];
			strcpy_s(nodeNameTmp, _MAX_PATH, nodeSel->name.data());
			ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_AutoSelectAll;
			if (ImGui::InputText("###", nodeNameTmp, _MAX_PATH, inputFlags))
			{
				nodeSel->name = nodeNameTmp;
			}
			
			ImGui::Spacing();
			ImGui::Text("Debug: ");
			ImGui::Spacing();
			ImGui::Separator();
			BTNode* parent = nodeSel->GetParent();
			if (parent)
				ImGui::Text("Parent: %i", parent->GetId());
			else
				ImGui::Text("Parent: -1");
			vector<BTNode*> childs = nodeSel->GetChilds();
			ImGui::Text("Childs: %i", childs.size());
			ImGui::Spacing();
			for (auto nodeChild : childs)
			{
				ImGui::Text("    * Node %i", nodeChild->GetId());
			}
			ImGui::Text("Subtree id: %i", nodeSel->GetSubtreeId());
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
			if (nodeType.typeId != 0) // Don't let the user create a Root node
			{
				if (ImGui::MenuItem(nodeType.name.data()))
				{
					bt->AddNode(cursorPosition.x, cursorPosition.y, nodeType.typeId);
				}
			}
		}
		ImGui::EndPopup();
	}

	// Node Options
	if (ImGui::BeginPopup("Node options"))
	{
		ne::ClearSelection();
		ne::SelectNode(selectedNodeId);
		if (ImGui::MenuItem("Remove"))
		{
			bt->RemoveNode(selectedNodeId);
			selectedNodeId = -1;
			ne::ClearSelection();
		}
		ImGui::EndPopup();
	}

	// Link Options
	if (ImGui::BeginPopup("Link options"))
	{
		ne::ClearSelection();
		ne::SelectLink(selectedLinkId);
		if(ImGui::MenuItem("Remove"))
		{
			bt->RemoveLink(selectedLinkId);
			selectedLinkId = -1;
			ne::ClearSelection();
		}
		ImGui::EndPopup();
	}
	ne::Resume();
}

void BeeTEditor::Menus()
{
	if (ne::ShowBackgroundContextMenu())
	{
		ImGui::OpenPopup("Create New Node");
	}
	if (ne::ShowNodeContextMenu(&selectedNodeId))
	{
		ImGui::OpenPopup("Node options");
	}
	if (ne::ShowLinkContextMenu(&selectedLinkId))
	{
		ImGui::OpenPopup("Link options");
	}
}

void BeeTEditor::Links()
{

	if (ne::BeginCreate(ImColor(255, 255, 255), 2.0f))
	{
		int startPinId = 0, endPinId = 0;
		if (ne::QueryNewLink(&startPinId, &endPinId))
		{
			BTPin* startPin = bt->FindPin(startPinId);
			BTPin* endPin = bt->FindPin(endPinId);

			if (startPin->kind == ne::PinKind::Target)
			{
				std::swap(startPin, endPin);
				std::swap(startPinId, endPinId);
			}

			if (startPin && endPin)
			{
				if (endPin->IsLinkAvailable() == false || startPin->IsLinkAvailable() == false)
				{
					// Maximum number of link connections made in input or output
					ne::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (endPin == startPin || endPin->kind == startPin->kind || startPin->node == endPin->node)
				{
					// Same Pin || Same kind || Same node
					ne::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (startPin->node->GetSubtreeId() == endPin->node->GetSubtreeId())
				{
					// Same tree it would create a loop
					ne::RejectNewItem(ImColor(255, 0, 0), 2.0f);
				}
				else if (ne::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
				{
					bt->AddLink(startPin, endPin);
				}
			}
		}
	}
	
	ne::EndCreate();
}
