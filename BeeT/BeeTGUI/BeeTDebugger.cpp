#include "BeeTDebugger.h"
#include "Application.h"
#include "Window.h"
#include "Network.h"
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "Log.h"
#include "BeeTGui.h"
#include "Data.h"
#include "dBehaviorTree.h"
#include "Blackboard.h"

#include "dSample.h"
#include "dsBBVar.h"

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

void BeeTDebugger::OpenNewConnection(int uid)
{
	//TODO
	btUID = uid;
}

void BeeTDebugger::HandleIncomingData(char * buf, int size, PacketType type)
{
	switch (type)
	{
	case PT_BT_FILE:
		LoadBT(buf, size);
		break;
	case PT_BT_UPDATE:
		UpdateBT(buf, size);
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

	if (bt)
	{
		bt->PrintSamples();
	}

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

	bt = new dBehaviorTree(btData); 
	bt->debugUID = btUID; //TODO

	g_app->beetGui->SetCurrentEditorContext(prevContext);
}

void BeeTDebugger::UpdateBT(const char * buf, int size)
{
	Data data(buf);
	int btUID = data.GetInt("uid");
	//TODO: Search bt for uid. Now is only the 'bt' variable
		
	int numSamples = data.GetArraySize("samples");
	for (int i = 0; i < numSamples; i++)
	{
		Data sData = data.GetArray("samples", i);
		SampleType sampleType = (SampleType)sData.GetInt("type");
		dSample* sample = nullptr;
		switch (sampleType)
		{
		case BBVAR_CHANGED:
			sample = (dSample*)SampleBBVar(bt, &sData); // TODO;
			break;
		case NODE_RETURNS:
			break;
		case NEW_CURRENT_NODE:
			break;
		case DECORATOR_CONDITION:
			break;
		}
		if (sample != nullptr)
			bt->AddSample(sample); // TODO
	}
}

dSample * BeeTDebugger::SampleBBVar(dBehaviorTree* bt, const Data * data)
{
	double timestamp = data->GetDouble("timestamp");
	dsBBVar* sample = new dsBBVar(bt, BBVAR_CHANGED, timestamp);

	sample->name = data->GetString("name");
	sample->varType = (BBVarType)data->GetInt("varType");

	switch (sample->varType)
	{
		case BV_BOOL:
			sample->oldValue = data->GetBool("oldValue");
			sample->newValue = data->GetBool("newValue");
			break;
		case BV_INT:
			sample->oldValue = data->GetInt("oldValue");
			sample->newValue = data->GetInt("newValue");
			break;
		case BV_FLOAT:
			sample->oldValue = data->GetFloat("oldValue");
			sample->newValue = data->GetFloat("newValue");
			break;
		case BV_STRING:
			sample->oldValue = string(data->GetString("oldValue"));
			sample->newValue = string(data->GetString("newValue"));
			break;
	}

	return sample;
}
