#ifndef __BEETEDITOR_H__
#define __BEETEDITOR_H__

#include "ThirdParty/ImGui/imgui.h"

class BehaviorTree;

class BeeTEditor
{
public:
	BeeTEditor();
	~BeeTEditor();

	bool Init();
	bool Update();
	bool CleanUp();

private:

	void Editor();
	void Inspector();

	// ImGui PopUps
	void ShowPopUps();

private:
	// For testing, a default behavior tree is already created. In the future, allow to create a new one or open an existent
	BehaviorTree* bt = nullptr;

	bool beetEditorWindowOpen = true;
	ImGuiWindowFlags flags =  ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
							| ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar
							| ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
							| ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse;
	int screenWidth = 0;
	int screenHeight = 0;

	ImVec2 editorSize;
	ImVec2 editorCanvasSize = ImVec2(0.75f, 1.0f);
	ImVec2 inspectorSize = ImVec2(0.25f, 1.0f);
};
#endif

