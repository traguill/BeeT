#ifndef __BEET_DEBUGGER_H__
#define __BEET_DEBUGGER_H__

#include "Packet.h"
#include "ThirdParty/ImGui/imgui.h"
#include <vector>
#include <stack>
#include <map>

class dBehaviorTree;
class Data;
class dSample;

class BeeTDebugger
{
public:
	BeeTDebugger();
	~BeeTDebugger();

	void Init();
	bool Update();
	void CleanUp();

	void OpenNewConnection(int uid); // Prepares a new slot to receive a new BT
	void HandleIncomingData(char* buf, int size, PacketType type);

	bool FocusNodeById(int nodeId);
private:

	// Windows
	void BlackboardWin();
	void CanvasWin();
	void MediPlayerWin();
	void HistoryWin(); // Deprecated
	void InspectorWin(); // Deprecated

	// Helpers
	void LoadBT(const char* buf, int size);
	void UpdateBT(const char* buf, int size);
	
private:
	int editorContextIDEmpty = -1;

	ImGuiWindowFlags flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse;

	int screenWidth = 0;
	int screenHeight = 0;

	ImVec2 debuggerSize;
	ImVec2 CanvasSize = ImVec2(0.8f, 0.9f);
	ImVec2 blackboardSize = ImVec2(0.2f, 0.9f);
	ImVec2 mediaplayerSize = ImVec2(1.0f, 0.1f);
	ImVec2 inspectorSize = ImVec2(0.2f, 0.6f); // Deprecated
	ImVec2 historySize = ImVec2(0.2f, 1.0f); // Deprecated


	std::vector<dBehaviorTree*> btList; // List of behavior trees opened
	dBehaviorTree* btCurrent; // Current bt selected
	std::stack<int> btSlots; // BT uids ready to handle connections

	std::map<int, dBehaviorTree*> btToDebugList;
};

#endif // !__BEET_DEBUGGER_H__
