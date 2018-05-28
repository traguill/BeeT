#include "BeeTDebugger.h"
#include "Application.h"
#include "Window.h"
#include "Network.h"
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "Log.h"
#include "BeeTGui.h"
#include "Data.h"
#include "BehaviorTree.h"
#include "Blackboard.h"

#include <string>

using namespace std;
namespace ne = ax::NodeEditor;

BeeTDebugger::BeeTDebugger()
{
}

BeeTDebugger::~BeeTDebugger()
{
}

void BeeTDebugger::Init()
{
	g_app->network->beetDebugger = this;
	editorContextId = g_app->beetGui->CreateNodeEditorContext();
}

bool BeeTDebugger::Update()
{
	g_app->window->GetWindowSize(screenWidth, screenHeight);
	debuggerSize.x = (float)screenWidth;
	debuggerSize.y = screenHeight - (ImGui::GetCursorPosY() - ImGui::GetCursorPosX());

	HistoryWin();
	CanvasWin();
	BlackboardWin();
	InspectorWin();
	
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
		LoadBT(buf, size);
		break;
	}
}

void BeeTDebugger::BlackboardWin()
{
	ImGui::SetNextWindowPos(ImVec2(debuggerSize.x - (debuggerSize.x * blackboardSize.x), ImGui::GetCursorPosY() - ImGui::GetCursorPosX()));
	ImGui::SetNextWindowSize(ImVec2(debuggerSize.x * blackboardSize.x, debuggerSize.y * blackboardSize.y));
	ImGui::Begin("Blackboard", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	
	if (bt)
	{
		for (auto var : bt->bb->variables)
		{
			ImGui::Text("%s ", var->name.data());
			ImGui::SameLine();
			switch (var->type)
			{
				case BV_BOOL:
				{
					bool valBool = boost::any_cast<bool>(var->value);
					ImGui::Text("%s", valBool ? "true" : "false");
				}
					break;
				case BV_INT:
				{
					int valInt = boost::any_cast<int>(var->value);
					ImGui::Text("%i", valInt);
				}
					break;
				case BV_FLOAT:
				{
					float valFloat = boost::any_cast<float>(var->value);
					ImGui::Text("%.2f", valFloat);
				}
					break;
				case BV_STRING:
				{
					string valString = boost::any_cast<string>(var->value);
					ImGui::Text("%s", valString.data());
				}
					break;
			}
		}
	}

	ImGui::End();
}

void BeeTDebugger::HistoryWin()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetCursorPosY() - ImGui::GetCursorPosX()));
	ImGui::SetNextWindowSize(ImVec2(debuggerSize.x * historySize.x, debuggerSize.y * historySize.y));
	ImGui::Begin("History", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImGui::Text("This is the history");

	ImGui::End();
}

void BeeTDebugger::InspectorWin()
{
	ImGui::SetNextWindowPos(ImVec2(debuggerSize.x - (debuggerSize.x * inspectorSize.x), ImGui::GetCursorPosY() - ImGui::GetCursorPosX() + (blackboardSize.y * debuggerSize.y)));
	ImGui::SetNextWindowSize(ImVec2(debuggerSize.x * inspectorSize.x, debuggerSize.y * inspectorSize.y));
	ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImGui::Text("This is the inspector");

	ImGui::End();
}

void BeeTDebugger::CanvasWin()
{
	ImGui::SetNextWindowPos(ImVec2(debuggerSize.x * historySize.x, ImGui::GetCursorPosY() - ImGui::GetCursorPosX()));
	ImGui::SetNextWindowSize(ImVec2(debuggerSize.x * CanvasSize.x, debuggerSize.y * CanvasSize.y));
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Canvas", nullptr, flags);

	g_app->beetGui->SetCurrentEditorContext(editorContextId); // Testing

	ne::Begin("BeeT Node Editor");
	if(bt)
		bt->Draw();

	ne::End();
	ImGui::End();
	ImGui::PopStyleVar();
}

void BeeTDebugger::LoadBT(const char* buffer, int size)
{
	ax::NodeEditor::EditorContext* prevContext = g_app->beetGui->GetNodeEditorContext();
	g_app->beetGui->SetCurrentEditorContext(editorContextId);
	Data btData(buffer);

	bt = new BehaviorTree(btData); 

	g_app->beetGui->SetCurrentEditorContext(prevContext);
}
