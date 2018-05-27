#ifndef __BEET_DEBUGGER_H__
#define __BEET_DEBUGGER_H__

#include "Packet.h"
#include "ThirdParty/ImGui/imgui.h"

class BehaviorTree;

class BeeTDebugger
{
public:
	BeeTDebugger();
	~BeeTDebugger();

	void Init();
	bool Update();
	void CleanUp();

	void HandleIncomingData(char* buf, int size, PacketType type);

private:

	// Windows
	void BlackboardWin();
	void HistoryWin();
	void InspectorWin();
	void CanvasWin();

	// Helpers
	void LoadBT(const char* buf, int size);

private:

	ImGuiWindowFlags flags = ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse;

	int screenWidth = 0;
	int screenHeight = 0;

	ImVec2 debuggerSize;
	ImVec2 CanvasSize = ImVec2(0.6f, 1.0f);
	ImVec2 inspectorSize = ImVec2(0.2f, 0.6f);
	ImVec2 blackboardSize = ImVec2(0.2f, 0.4f);
	ImVec2 historySize = ImVec2(0.2f, 1.0f);

	// Testing
	int editorContextId = 0;
	BehaviorTree* bt = nullptr;
};

#endif // !__BEET_DEBUGGER_H__
