#ifndef __BEETEDITOR_H__
#define __BEETEDITOR_H__

#include "ThirdParty/ImGui/imgui.h"
#include <string>
#include <map>
#include <vector>

class BehaviorTree;
class Blackboard;
class Data;
class ItemList;
struct ListObject;
enum BBVarType;

class BeeTEditor
{
public:
	BeeTEditor();
	~BeeTEditor();

	bool Init();
	bool Update();
	bool CleanUp();

	void Serialize(const char* filename)const;
	void Load(const char* path);

	void NewBehaviorTree(Data* data = nullptr);

	static void CallBackAddNode(void* obj, const std::string& category, const std::string& item, int additionalData);
	static void CallBackBBVarType(void* obj, const std::string& category, const std::string& item, int additionalData);
	static void CallBackBBVarList(void* obj, const std::string& category, const std::string& item, int additionalData);

private:

	void BlackboardWindow();
	void BlackboardVarDetail();
	void Editor();
	void Inspector();
	void BehaviortreeDetail();

	// ImGui PopUps
	void ShowPopUps();

	// Editor helpers
	void Menus(); //Background/Node/Link/Pin
	void Links();
	void UpdateSelection();
	void CloseBehaviorTree();
	void ClearAllSelection();

	void InitBBListCategories();
	void ListAllBBVars();

private:
	std::vector<BehaviorTree*> btList; // List of behavior trees opened
	BehaviorTree* btCurrent; // Current bt selected

	bool beetEditorWindowOpen = true;
	ImGuiWindowFlags flags =  ImGuiWindowFlags_::ImGuiWindowFlags_NoResize
							| ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar
							| ImGuiWindowFlags_::ImGuiWindowFlags_NoMove
							| ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse;
	int screenWidth = 0;
	int screenHeight = 0;

	ImVec2 editorSize;
	ImVec2 editorCanvasSize = ImVec2(0.6f, 1.0f);
	ImVec2 inspectorSize = ImVec2(0.2f, 0.8f);
	ImVec2 blackboardSize = ImVec2(0.2f, 0.8f);
	ImVec2 blackboardVarDetailSize = ImVec2(0.2f, 0.2f);
	ImVec2 btDetailSize = ImVec2(0.2f, 0.2f);

	// Widgets
	ItemList* widgetItemList = nullptr;
	ItemList* widgetBBList = nullptr;
	ListObject* bbVarTypeObj = nullptr;
	std::map<std::string, BBVarType> bbVarTypeConversor;

	ItemList* widgetBBVars = nullptr;
	ListObject* bbVarListObj = nullptr;

	// Inspector
	int selectedNodeId = -1;
	int selectedLinkId = -1;

	bool nodeAddedFlag = false;

	// Blackboard
	int bbvarSelected = -1; // Selected to edit its name
	bool bbvarSetFocus = false; // To set the focus on the bbVar next frame
	int bbvarValueSelected = -1; // selected to change its value
};
#endif

