#include "BeeTDebugger.h"
#include "Application.h"
#include "Window.h"
#include "Network.h"
#include "Log.h"
#include "BeeTGui.h"
#include "Data.h"
#include "dBehaviorTree.h"
#include "Blackboard.h"

#include "dSample.h"
#include "dsBBVar.h"
#include "dsNewCurrent.h"
#include "dsReturnStatus.h"
#include "dsBTEnd.h"
#include "dsDecoratorCondition.h"

#include <string>
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "IconsFont.h"
#include "ThirdParty/ImGui/imgui_tabs.h"

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
	editorContextIDEmpty = g_app->beetGui->CreateNodeEditorContext();
}

bool BeeTDebugger::Update()
{
	g_app->window->GetWindowSize(screenWidth, screenHeight);
	debuggerSize.x = (float)screenWidth;
	debuggerSize.y = screenHeight - (ImGui::GetCursorPosY() - ImGui::GetCursorPosX());

	CanvasWin();
	BlackboardWin();
	MediPlayerWin();
	
	return true;
}

void BeeTDebugger::CleanUp()
{
	for (auto bt : btList)
		delete bt;
}

void BeeTDebugger::OpenNewConnection(int uid)
{
	btSlots.push(uid);
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

bool BeeTDebugger::FocusNodeById(int nodeId)
{
	if (btCurrent)
	{
		if (btCurrent->FindNode(nodeId))
		{
			ne::ClearSelection();
			ne::SelectNode(nodeId);
			ne::NavigateToSelection();
			return true;
		}
	}
	return false;
}

void BeeTDebugger::BlackboardWin()
{
	ImGui::SetNextWindowPos(ImVec2(debuggerSize.x - (debuggerSize.x * blackboardSize.x), ImGui::GetCursorPosY() - ImGui::GetCursorPosX()));
	ImGui::SetNextWindowSize(ImVec2(debuggerSize.x * blackboardSize.x, debuggerSize.y * blackboardSize.y));
	ImGui::Begin("Blackboard", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	
	if (btCurrent)
	{
		for (auto var : btCurrent->bb->variables)
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

	if (btCurrent)
	{
		btCurrent->PrintSamples();
	}

	ImGui::End();
}

void BeeTDebugger::InspectorWin()
{

}

void BeeTDebugger::CanvasWin()
{
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetCursorPosY() - ImGui::GetCursorPosX()));
	ImGui::SetNextWindowSize(ImVec2(debuggerSize.x * CanvasSize.x, debuggerSize.y * CanvasSize.y));
	ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Canvas", nullptr, flags);

	if (btList.size() == 0)
	{
		g_app->beetGui->SetCurrentEditorContext(editorContextIDEmpty);
		ne::Begin("BeeT Node Editor");
		ne::End();
		ImGui::End();
		ImGui::PopStyleVar();
		return;
	}
	
	ImGui::BeginTabBar("###btDebug_tab_bar");
	ImGui::DrawTabsBackground();
	ImGui::SetCursorPosX(0.0f);

	for (auto bt : btList)
	{
		ImGui::PushID(bt->GetUID());

		if (ImGui::AddTab(bt->filename.data()))
		{
			btCurrent = bt;
			g_app->beetGui->SetCurrentEditorContext(bt->editorId);

			ne::Begin("BeeT Node Editor");
			bt->Draw();
			ne::End();
		}
		ImGui::PopID();
	}

	ImGui::EndTabBar();
	ImGui::End();
	ImGui::PopStyleVar();
}

void BeeTDebugger::MediPlayerWin()
{
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetCursorPosY() - ImGui::GetCursorPosX() + (blackboardSize.y * debuggerSize.y)));
	ImGui::SetNextWindowSize(ImVec2(debuggerSize.x * mediaplayerSize.x, debuggerSize.y * mediaplayerSize.y));
	ImGui::Begin("MediaPlayer", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar);

	if (btCurrent == nullptr)
	{
		ImGui::End();
		return;
	}
	if (btCurrent->sampleSelected == -1)
	{
		btCurrent->ApplySampleEffect(0);
	}

	if (ImGui::Button("Prev###media_prev"))
	{
		if (btCurrent->sampleSelected > 0)
		{
			btCurrent->ApplySampleEffect(btCurrent->sampleSelected - 1);
		}		
	}
	ImGui::SameLine();
	if(ImGui::Button(ICON_PLAY))
	{

	}
	ImGui::SameLine();
	if (ImGui::Button("Next###media_next"))
	{
		if (btCurrent->sampleSelected < btCurrent->changes.size() - 1)
		{
			btCurrent->ApplySampleEffect(btCurrent->sampleSelected + 1);
		}				
	}

	int tmpInt = btCurrent->sampleSelected;
	if (ImGui::SliderInt("###timeline", &tmpInt, 0, btCurrent->changes.size() - 1))
	{
		btCurrent->ApplySampleEffect(tmpInt);
	}
	ImGui::SameLine();
	ImGui::Text(" %i of %i", tmpInt, btCurrent->changes.size() - 1);

	ImGui::End();
}

void BeeTDebugger::LoadBT(const char* buffer, int size)
{
	ax::NodeEditor::EditorContext* prevContext = g_app->beetGui->GetNodeEditorContext();
	int editorId = g_app->beetGui->CreateNodeEditorContext();
	g_app->beetGui->SetCurrentEditorContext(editorId);
	Data btData(buffer);

	dBehaviorTree* bt = new dBehaviorTree(btData);
	assert(btSlots.size() > 0); // OpenNewConnection should have been called first!
	bt->debugUID = btSlots.top();
	btSlots.pop();
	bt->editorId = editorId;

	btToDebugList.insert(std::pair<int, dBehaviorTree*>(bt->debugUID, bt));
	btList.push_back(bt);

	g_app->beetGui->SetCurrentEditorContext(prevContext);
}

void BeeTDebugger::UpdateBT(const char * buf, int size)
{
	Data data(buf);
	int btUID = data.GetInt("uid");
	
	auto btFound = btToDebugList.find(btUID);

	if (btFound == btToDebugList.end())
	{
		LOGE("Debug BT with id %i not found", btUID);
		return;
	}

	dBehaviorTree* bt = btFound->second;
		
	int numSamples = data.GetArraySize("samples");
	for (int i = 0; i < numSamples; i++)
	{
		Data sData = data.GetArray("samples", i);
		SampleType sampleType = (SampleType)sData.GetInt("type");
		dSample* sample = nullptr;
		switch (sampleType)
		{
		case BBVAR_CHANGED:
			sample = (dSample*)new dsBBVar(bt, sData);
			break;
		case NODE_RETURNS:
			sample = (dSample*)new dsReturnStatus(bt, sData);
			break;
		case NEW_CURRENT_NODE:
			sample = (dSample*) new dsNewCurrent(bt, sData);
			break;
		case DECORATOR_CONDITION:
			//sample = (dSample*) new dsDecoratorCondition(bt, sData); TODO:IGNORE FOR NOW. TOO MUCH SPAM. FIX THIS!!!
			break;
		case BT_END:
			sample = (dSample*) new dsBTEnd(bt, sData);
			break;
		}
		if (sample != nullptr)
			bt->AddSample(sample);
	}
}
