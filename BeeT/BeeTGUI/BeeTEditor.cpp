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
#include "ItemList.h"
#include "Blackboard.h"
#include "BTDecorator.h"

#include "ThirdParty/ImGui/imgui_tabs.h"
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

	//btCurrent = new BehaviorTree();
	//btList.push_back(btCurrent);

	widgetItemList = new ItemList();
	widgetBBList = new ItemList();
	InitBBListCategories();
	widgetBBVars = new ItemList();

	return true;
}

bool BeeTEditor::Update()
{
	g_app->window->GetWindowSize(screenWidth, screenHeight);
	editorSize.x = (float) screenWidth;
	editorSize.y = screenHeight - (ImGui::GetCursorPosY() - ImGui::GetCursorPosX());

	BlackboardWindow();
	BlackboardVarDetail();

	Editor();
	
	UpdateSelection();
	Inspector();
	BehaviortreeDetail();

	return true;
}

bool BeeTEditor::CleanUp()
{
	for (auto bt : btList)
		delete bt;

	delete widgetItemList;
	delete bbVarTypeObj;
	delete widgetBBList;
	delete widgetBBVars;
	return true;
}

void BeeTEditor::Serialize(const char* filename) const
{
	char* buffer = nullptr;
	string btName = g_app->fileSystem->GetFileNameFromPath(filename);
	btCurrent->filename = btName;
	int size = btCurrent->Serialize(&buffer);
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

		NewBehaviorTree(&btData); 

		string btName = g_app->fileSystem->GetFileNameFromPath(path);
		btList.back()->filename = btName;
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
	ax::NodeEditor::EditorContext* prevContext = g_app->beetGui->GetNodeEditorContext();
	int editorId = g_app->beetGui->CreateNodeEditorContext();
	g_app->beetGui->SetCurrentEditorContext(editorId);
	
	BehaviorTree* bt;
	if (data)
	{
		bt = new BehaviorTree(*data);
	}
	else
	{
		bt = new BehaviorTree();
	}
	btList.push_back(bt);
	ClearAllSelection();
	bt->editorId = editorId;

	if (btCurrent == nullptr)
		btCurrent = bt;

	// Reset the context to the old one
	g_app->beetGui->SetCurrentEditorContext(prevContext);
}

void BeeTEditor::CallBackAddNode(void * obj, const std::string & category, const std::string & item, int additionalData)
{
	BeeTEditor* editor = ((BeeTEditor*)obj);
	if (editor && g_app && g_app->beetGui)
	{
		int id = g_app->beetGui->btNodeTypes->GetNodeTypeId(category, item);
		if (id != -1)
		{
			ImVec2 pos = ne::ScreenToCanvas(ImGui::GetMousePos());
			int nodeId = editor->btCurrent->AddNode(pos.x, pos.y, id);
			editor->selectedNodeId = nodeId;
			ne::ClearSelection();
			ne::SelectNode(nodeId);
			editor->nodeAddedFlag = true;
		}
	}
}

void BeeTEditor::CallBackBBVarType(void * obj, const std::string & category, const std::string & item, int additionalData)
{
	BeeTEditor* editor = (BeeTEditor*)obj;
	if (editor)
	{
		map<string, BBVarType>::iterator it = editor->bbVarTypeConversor.find(item);
		if (it != editor->bbVarTypeConversor.end())
		{
			editor->btCurrent->bb->ChangeVarType(additionalData, it->second);
		}
	}
}

void BeeTEditor::CallBackBBVarList(void * obj, const std::string & category, const std::string & item, int additionalData)
{
	BeeTEditor* editor = (BeeTEditor*)obj;
	if (editor)
	{
		delete editor->bbVarListObj;
		editor->bbVarListObj = nullptr;

		BBVar* var = editor->btCurrent->bb->FindVar(item);

		// TODO: For now only Blackboard comparisons
		editor->btCurrent->AddDecorator(editor->selectedNodeId, var); // TODO: SEND DECORATOR NAME
	}
}

bool BeeTEditor::FoucsNodeById(int nodeId)
{
	if (btCurrent)
	{
		if (btCurrent->FindNode(nodeId))
		{
			selectedNodeId = nodeId;
			ne::ClearSelection();
			ne::SelectNode(nodeId);
			ne::NavigateToSelection();
			return true;
		}
	}
	return false;
}

void BeeTEditor::BlackboardWindow()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetCursorPosY() - ImGui::GetCursorPosX()));
	ImGui::SetNextWindowSize(ImVec2(editorSize.x * blackboardSize.x, editorSize.y * blackboardSize.y));
	ImGui::Begin("   BlackBoard", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	
	if (widgetBBList->IsVisible())
		widgetBBList->Draw();

	if (btCurrent == nullptr)
	{
		ImGui::End();
		return;
	}

	Blackboard* bb = btCurrent->bb;

	for (int i = 0; i < bb->variables.size(); ++i)
	{
		BBVar* bbvar = bb->variables[i];
		// Var TYPE
		switch (bbvar->type)
		{
		case BV_BOOL:
			ImGui::Text("bool   ");
			break;
		case BV_INT:
			ImGui::Text("int    ");
			break;
		case BV_FLOAT:
			ImGui::Text("float  ");
			break;
		case BV_STRING:
			ImGui::Text("string ");
			break;
		default:
			ImGui::Text("Error  ");
			break;
		}
		
		if (ImGui::IsItemClicked()) // Change BBVar type
		{
			if (bbvar->decorators.size() == 0) // Only allow change the type when BBVar is not linked to any decorator
			{
				widgetBBList->SetWidgetPosition(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
				widgetBBList->SetSelFunctionCallback(BeeTEditor::CallBackBBVarType, this, i);
				widgetBBList->SetVisible(true, bbVarTypeObj);
			}
		}

		ImGui::SameLine();

		if (bbvarSelected == i) // Edit name
		{
			char varNameTmp[_MAX_PATH];
			strcpy_s(varNameTmp, _MAX_PATH, bbvar->name.data());
			ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;
			ImGui::PushID(i);
			if (bbvarSetFocus)	
				ImGui::SetKeyboardFocusHere(0);
		
			if (ImGui::InputText("###", varNameTmp, _MAX_PATH, inputFlags))
			{
				if (bb->IsVarNameUnique(varNameTmp))
					bbvar->name = varNameTmp;
				bbvarSelected = -1;
			}	
			if (ImGui::IsItemActive() == false && bbvarSetFocus == false)
			{
				bbvarSelected = -1;
				bbvarValueSelected = i;
			}
			if (bbvarSetFocus)
				bbvarSetFocus = false;
			ImGui::PopID();
		}
		else // Display name as selectable
		{
			bool isSelected = i == bbvarValueSelected ? true : false;
			if (ImGui::Selectable(bbvar->name.data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (ImGui::IsMouseDoubleClicked(0))
				{
					bbvarSelected = i;
					bbvarSetFocus = true;
				}
				else // Single click
				{
					bbvarValueSelected = i;
				}
			}
		}	
	}
	if (ImGui::Button("Add New"))
	{
		bb->CreateDummyVar();
		bbvarSelected = bbvarValueSelected = bb->variables.size() - 1;
		bbvarSetFocus = true;
	}

	ImGui::End();
}

void BeeTEditor::BlackboardVarDetail()
{
	ImGui::SetNextWindowPos(ImVec2(0.0f, (ImGui::GetCursorPosY() - ImGui::GetCursorPosX()) + (blackboardSize.y * editorSize.y)));
	ImGui::SetNextWindowSize(ImVec2(editorSize.x * blackboardVarDetailSize.x, editorSize.y * blackboardVarDetailSize.y));
	ImGui::Begin("   BlackBoard Variable", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
	if (btCurrent == nullptr)
	{
		ImGui::End();
		return;
	}
	Blackboard* bb = btCurrent->bb;
	if (bbvarValueSelected != -1)
	{
		BBVar* var = bb->variables[bbvarValueSelected];
		ImGui::Text("%s", var->name.data());
		
		ImGui::Spacing();
		
		switch (var->type)
		{
		case BV_BOOL:
		{
			ImGui::Text("bool   "); ImGui::SameLine();
			bool valBool = boost::any_cast<bool>(var->value);
			if (ImGui::Checkbox("###bvboolinput", &valBool))
			{
				var->value = valBool;
			}
		}
			break;
		case BV_INT:
		{
			ImGui::Text("int    "); ImGui::SameLine();
			int valInt = boost::any_cast<int>(var->value);
			if (ImGui::InputInt("###bvintinput", &valInt))
			{
				var->value = valInt;
			}
		}
			break;
		case BV_FLOAT:
		{
			ImGui::Text("float  "); ImGui::SameLine();
			float valFloat = boost::any_cast<float>(var->value);
			if (ImGui::InputFloat("###bvfloatinput", &valFloat))
			{
				var->value = valFloat;
			}
		}
			break;
		case BV_STRING:
			{
				ImGui::Text("string "); ImGui::SameLine();
				string valString = boost::any_cast<string>(var->value);
				char varNameTmp[_MAX_PATH];
				strcpy_s(varNameTmp, _MAX_PATH, valString.data());
				if (ImGui::InputText("###bvstringinput", varNameTmp, _MAX_PATH))
				{
					var->value = string(varNameTmp);
				}
			}
			break;
		default:
			ImGui::Text("Error  "); 
			break;
		}
		
		if (ImGui::Button("Remove###bbdetailremove"))
		{
			bb->RemoveVar(bbvarValueSelected);
			bbvarSelected = -1;
			bbvarSetFocus = false;
			bbvarValueSelected = -1;
		}
	}

	ImGui::End();
}

void BeeTEditor::Editor()
{
	ImGui::SetNextWindowPos(ImVec2(screenWidth * blackboardSize.x, ImGui::GetCursorPosY() - ImGui::GetCursorPosX())); // The Y component substracts the cursorX position because imgui by default has margins
	ImGui::SetNextWindowSize(ImVec2(editorSize.x * editorCanvasSize.x, editorSize.y * editorCanvasSize.y));
	
	ImGui::Begin("BeeT Editor Window", &beetEditorWindowOpen, flags);

	if (btCurrent == nullptr)
	{
		float w = ImGui::GetWindowWidth();
		float h = ImGui::GetWindowHeight();
		ImGui::SetCursorPosX(w*0.5f);
		ImGui::SetCursorPosY(h*0.5f);
		if (ImGui::Button("Create new"))
		{
			NewBehaviorTree();
		}
		ImGui::End();
		return;
	}

	ImGui::BeginTabBar("###bt_tab_bar");
	ImGui::DrawTabsBackground();
	ImGui::SetCursorPosX(0.0f);

	for (auto bt : btList)
	{
		ImGui::PushID(bt->GetUID());
		
		if (ImGui::AddTab(bt->filename.data()))
		{
			if (btCurrent != bt)
				ClearAllSelection();
			btCurrent = bt;
			g_app->beetGui->SetCurrentEditorContext(bt->editorId);
			ne::Begin("BeeT Node Editor");

			Menus();
			btCurrent->Draw();
			Links();

			// Node Editor Suspended -----------------------------
			ne::Suspend();
			ShowPopUps();

			if (widgetItemList->IsVisible())
				widgetItemList->Draw();

			if (widgetBBVars->IsVisible())
			{
				widgetBBVars->Draw();
			}
			else if (bbVarListObj != nullptr)
			{
				delete bbVarListObj;
				bbVarListObj = nullptr;
			}
			ne::Resume();
			// ---------------------------------------------------

			ne::End(); // BeeT Node Editor
		}
		ImGui::PopID();
	}
	ImGui::EndTabBar();
	ImGui::End();
}

void BeeTEditor::Inspector()
{
	ImGui::SetNextWindowPos(ImVec2(screenWidth * (blackboardSize.x + editorCanvasSize.x), ImGui::GetCursorPosY() - ImGui::GetCursorPosX()));
	ImGui::SetNextWindowSize(ImVec2(editorSize.x * inspectorSize.x, editorSize.y * inspectorSize.y));
	ImGui::Begin("   Inspector", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	if (selectedNodeId != -1)
	{
		BTNode* nodeSel = btCurrent->FindNode(selectedNodeId);
		if (nodeSel)
		{
			ImGui::Text("Type: %s", nodeSel->type->name.data());
			
			ImGui::Text("Name: "); ImGui::SameLine();
			char nodeNameTmp[_MAX_PATH];
			strcpy_s(nodeNameTmp, _MAX_PATH, nodeSel->name.data());
			ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_AutoSelectAll;
			if (nodeAddedFlag)
			{
				ImGui::SetKeyboardFocusHere();
				nodeAddedFlag = false;
			}
			if (ImGui::InputText("###", nodeNameTmp, _MAX_PATH, inputFlags))
			{
				nodeSel->name = nodeNameTmp;
			}

			ImGui::Spacing();

			if (nodeSel->decorators.size() > 0)
			{
				ImGui::Dummy(ImVec2(0.0f, 20.0f));
				ImGui::Text("Decorators: ");
				ImGui::Separator();
			}
			for (auto dec : nodeSel->decorators)
			{
				dec->InspectorInfo();
				ImGui::Separator();
			}
			ImGui::Spacing();
			
			nodeSel->InspectorInfo();
		}
	}

	ImGui::End();
}

void BeeTEditor::BehaviortreeDetail()
{
	ImGui::SetNextWindowPos(ImVec2(screenWidth * (blackboardSize.x + editorCanvasSize.x), (ImGui::GetCursorPosY() - ImGui::GetCursorPosX()) + (inspectorSize.y * editorSize.y)));
	ImGui::SetNextWindowSize(ImVec2(editorSize.x * btDetailSize.x, editorSize.y * btDetailSize.y));
	ImGui::Begin("   Behavior Tree Details", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	if (btCurrent) // This should never be null, but just in case
	{
		ImGui::Text("File Name: ");

		char btNameTmp[_MAX_PATH];
		strcpy_s(btNameTmp, _MAX_PATH, btCurrent->filename.data());
		if (ImGui::InputText("###btinputtext", btNameTmp, _MAX_PATH, ImGuiInputTextFlags_AutoSelectAll))
		{
			btCurrent->filename = btNameTmp;
		}

		ImGui::Text("Options: ");
		if (ImGui::Button("Save"))
		{
			string filenamepath = btCurrent->filename + ".json";
			g_app->beetGui->SaveFile(g_app->beetGui, filenamepath.data());
		}
		
		ImGui::SameLine();

		if (ImGui::Button("Save and Close"))
		{
			string filenamepath = btCurrent->filename + ".json";
			g_app->beetGui->SaveFile(g_app->beetGui, filenamepath.data());
			CloseBehaviorTree();
		}
		ImGui::SameLine();
		if (ImGui::Button("Close"))
		{
			CloseBehaviorTree();
		}
	}
	ImGui::End();
}

void BeeTEditor::ShowPopUps()
{
	// Node Options
	if (ImGui::BeginPopup("Node options"))
	{
		ne::ClearSelection();
		ne::SelectNode(selectedNodeId);
		if (ImGui::MenuItem("Remove"))	// Remove
		{
			btCurrent->RemoveNode(selectedNodeId);
			selectedNodeId = -1;
			ne::ClearSelection();
		}

		if (btCurrent->bb->variables.size() > 0)
		{
			if(ImGui::MenuItem("Add Decorator")) // Add Decorator
			{
				ListAllBBVars();
			}
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
			btCurrent->RemoveLink(selectedLinkId);
			selectedLinkId = -1;
			ne::ClearSelection();
		}
		ImGui::EndPopup();
	}
}

void BeeTEditor::Menus()
{
	if (ne::ShowBackgroundContextMenu())
	{
		widgetItemList->SetVisible(true, g_app->beetGui->btNodeTypes->GetListObjectPtr());	// Activate ItemList
		widgetItemList->SetSelFunctionCallback(BeeTEditor::CallBackAddNode, this);			// Link Select Function
		ImVec2 mPos = ne::CanvasToScreen(ImGui::GetMousePos());
		widgetItemList->SetWidgetPosition(mPos.x, mPos.y);
	}
	if (ne::ShowNodeContextMenu(&selectedNodeId))
	{
		if(btCurrent->IsRoot(selectedNodeId) == false)
			ImGui::OpenPopup("Node options");
		else
		{
			ne::ClearSelection();
			selectedNodeId = -1;
		}
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
			BTPin* startPin = btCurrent->FindPin(startPinId);
			BTPin* endPin = btCurrent->FindPin(endPinId);

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
					btCurrent->AddLink(startPin, endPin);
				}
			}
		}
	}
	
	ne::EndCreate();
}

void BeeTEditor::UpdateSelection()
{
	// Get the node selected at this frame. ALWAYS remove root node from selection
	if (ne::HasSelectionChanged())
	{
		int selCount = ne::GetSelectedObjectCount();
		if (selCount == 1)
		{
			ne::GetSelectedNodes(&selectedNodeId, 1);
			if (btCurrent->IsRoot(selectedNodeId))
			{
				selectedNodeId = -1;
				ne::ClearSelection();
			}
		}
		else
		{
			selectedNodeId = -1;
			vector<int> selNodes;
			selNodes.resize(selCount);

			ne::GetSelectedNodes(selNodes.data(), selCount);
			for (int i = 0; i < selCount; i++)
			{
				if (btCurrent->IsRoot(selNodes[i]))
				{
					ne::DeselectNode(selNodes[i]);
					break;
				}
			}
		}
	}

}

void BeeTEditor::CloseBehaviorTree()
{
	assert(btCurrent != nullptr);
	auto btIt = std::find(btList.begin(), btList.end(), btCurrent);
	assert(btIt != btList.end());
	btList.erase(btIt);
	delete btCurrent;
	btCurrent = nullptr;
	if(btList.size() > 0)
		btCurrent = btList[0];
	ClearAllSelection();
}

void BeeTEditor::ClearAllSelection()
{
	selectedNodeId = -1;
	selectedLinkId = -1;
	nodeAddedFlag = false;
	bbvarSelected = -1;
	bbvarSetFocus = false;
	bbvarValueSelected = -1;
}

void BeeTEditor::InitBBListCategories()
{
	bbVarTypeObj = new ListObject();

	bbVarTypeObj->AddItemInCategory("Basic", "Boolean");
	bbVarTypeConversor.insert(pair<string, BBVarType>("Boolean", BV_BOOL));
	bbVarTypeObj->AddItemInCategory("Basic", "Integer");
	bbVarTypeConversor.insert(pair<string, BBVarType>("Integer", BV_INT));
	bbVarTypeObj->AddItemInCategory("Basic", "Float");
	bbVarTypeConversor.insert(pair<string, BBVarType>("Float", BV_FLOAT));
	bbVarTypeObj->AddItemInCategory("Basic", "String");
	bbVarTypeConversor.insert(pair<string, BBVarType>("String", BV_STRING));

	bbVarTypeObj->SortAll();
}

void BeeTEditor::ListAllBBVars()
{
	if (bbVarListObj == nullptr)
	{
		bbVarListObj = new ListObject();
		for (auto var : btCurrent->bb->variables)
		{
			bbVarListObj->AddItemInCategory("Blackboard", var->name.data());
		}
		widgetBBVars->SetWidgetPosition(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
		widgetBBVars->SetSelFunctionCallback(BeeTEditor::CallBackBBVarList, this);
		widgetBBVars->SetVisible(true, bbVarListObj);
	}
}
