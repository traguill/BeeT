#include "BTNode.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "ThirdParty/NodeEditor/Source/Shared/Interop.h"
#include "Log.h"
#include "Application.h"
#include "BeeTGui.h"
#include "BehaviorTree.h"
#include "Random.h"
#include "BTDecorator.h"
#include "Blackboard.h"
#include "BeeTEditor.h"

#include <algorithm>

namespace ne = ax::NodeEditor;

BTNode::BTNode()
{
}

BTNode::BTNode(int id, int sourcePinId, int targetPinId, int typeId, BehaviorTree* bt, BTNode* parent) : id(id), bt(bt), parent(parent)
{
	type = g_app->beetGui->btNodeTypes->GetTypeById(typeId);
	if(type)
		name = type->name;

	inputPin = new BTPin(targetPinId, ne::PinKind::Target, this, false);
	outputPin = new BTPin(sourcePinId, ne::PinKind::Source, this, false);

	InitExtraData();

	ReloadSubtreeId();
}

BTNode::BTNode(BehaviorTree* bt, Data & data) : bt(bt)
{
	id = data.GetInt("id");
	int parentId = data.GetInt("parentId");
	parent = parentId > 0 ? bt->FindNode(parentId) : nullptr;
	type = g_app->beetGui->btNodeTypes->GetTypeById(data.GetInt("type"));

	inputPin = new BTPin(this, data.GetArray("pins", 0));
	outputPin = new BTPin(this, data.GetArray("pins", 1));

	name = data.GetString("name");

	int numDecorators = data.GetArraySize("decorators");

	for (int i = 0; i < numDecorators; ++i)
	{
		BTDecorator* dec = new BTDecorator(this, bt->bb, data.GetArray("decorators", i));
		decorators.push_back(dec);
	}

	switch (type->typeId)
	{
		//case 0: // Root
		//case 1: // Selector
		//case 2: // Sequence
		//case 3: // Parallel
	case 4: // Custom Task
	{
		const char* s = data.GetString("extraData");
		if(s)
			extraData = bt->bb->FindVar(s);
	}
		break;
	case 5: // Wait
	{
		extraData = new char[sizeof(float)];
		float tmpExtraData = data.GetFloat("extraData");
		memcpy(extraData, &tmpExtraData, sizeof(float));
	}
		break;
	}

	ReloadSubtreeId();
}

BTNode::~BTNode()
{
	for (auto dec : decorators)
		delete dec;
	delete inputPin;
	delete outputPin;
	if (extraData)
		if(type->typeId == 5) // Wait node
			delete[] extraData;
		else if (type->typeId == 6)
		{
			BBVar* var = (BBVar*)extraData;
			var->nodes.erase(std::find(var->nodes.begin(), var->nodes.end(), this));
		}
			
}

std::vector<BTLink*> BTNode::GetAllLinks()
{
	std::vector<BTLink*> linkList;
	for (std::vector<BTLink*>::iterator link = inputPin->links.begin(); link != inputPin->links.end(); ++link)
		linkList.push_back(*link);
	
	for (std::vector<BTLink*>::iterator link = outputPin->links.begin(); link != outputPin->links.end(); ++link)
		linkList.push_back(*link);
	
	return linkList;
}

int BTNode::GetSubtreeId() const
{
	return subtreeId;
}

void BTNode::PrepareToDraw()
{
	// Remove decorators now (if any)
	if (decoratorsToRemove.empty() == false)
	{
		for (auto dec : decoratorsToRemove)
		{
			dec->CleanUp();
			decorators.erase(std::find(decorators.begin(), decorators.end(), dec));
			delete dec;
		}
		decoratorsToRemove.clear();
	}
	// --------------------------------

	ne::BeginNode(id);
	ImGui::BeginVertical(id);

	// Input ---------------------------------
	ImGui::BeginHorizontal("inputs");
	ImGui::Spring(0, padding * 2);

	if (type->typeId != 0) // Check if is not root (type 0)
	{
		ImGui::Dummy(ImVec2(0, padding));
		ImGui::Spring(1, 0);
		inputsRect = ImGui_GetItemRect();

		ne::PushStyleVar(ne::StyleVar_PinArrowSize, 10.0f);
		ne::PushStyleVar(ne::StyleVar_PinArrowWidth, 10.0f);
		ne::PushStyleVar(ne::StyleVar_PinCorners, 12);
		ne::BeginPin(inputPin->id, ne::PinKind::Target);
		ne::PinPivotRect(to_imvec(inputsRect.top_left()), to_imvec(inputsRect.bottom_right()));
		ne::PinRect(to_imvec(inputsRect.top_left()), to_imvec(inputsRect.bottom_right()));
		ne::EndPin();
		ne::PopStyleVar(3);
	}
	else
		ImGui::Dummy(ImVec2(0, padding));

	ImGui::Spring(0, padding * 2);
	ImGui::EndHorizontal(); // 'inputs'
	// -----------------------------------------

	// Content ---------------------------------
	ImGui::BeginHorizontal("content_frame");
	ImGui::Spring(1, padding);

	ImGui::BeginVertical("contentBig", ImVec2(0.0f, 0.0f));

	for (auto dec : decorators)
	{
		dec->PrepareToDraw();
	}
	ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
	ImGui::Dummy(ImVec2(160, 0));

	ImGui::Spring(1);
	BBVar* data = (BBVar*)extraData;
	if (type->typeId == 5)
		PrepToDrawWait();
	else if (type->typeId == 4 && extraData != nullptr)
		PrepToDrawTask();
	else
	{
		if (g_app->beetGui->showNodeId)
			ImGui::Text("(%i)%s%s", id, type->icon.data(), name.data());
		else
			ImGui::Text("%s%s", type->icon.data(), name.data());
	}
	
	ImGui::Spring(1);

	ImGui::EndVertical(); // 'content'

	contentRect = ImGui_GetItemRect();
	ImGui::EndVertical(); //content big
	ImGui::Spring(1, padding);
	ImGui::EndHorizontal(); // 'content_frame'
	// -------------------------------------------

	// Output ------------------------------------
	ImGui::BeginHorizontal("outputs");
	ImGui::Spring(0, padding * 2);

	if (type->maxOutputs != 0)
	{
		ImGui::Dummy(ImVec2(0, padding));
		ImGui::Spring(1, 0);
		outputsRect = ImGui_GetItemRect();

		ne::PushStyleVar(ne::StyleVar_PinCorners, 3);
		ne::BeginPin(outputPin->id, ne::PinKind::Source);
		ne::PinPivotRect(to_imvec(outputsRect.top_left()), to_imvec(outputsRect.bottom_right()));
		ne::PinRect(to_imvec(outputsRect.top_left()), to_imvec(outputsRect.bottom_right()));
		ne::EndPin();
		ne::PopStyleVar();
	}
	else
		ImGui::Dummy(ImVec2(0, padding));

	ImGui::Spring(0, padding * 2);
	ImGui::EndHorizontal(); // 'outputs'
	// -------------------------------------------

	ImGui::EndVertical(); // nodeId
	ne::EndNode();
}

void BTNode::InspectorInfo()
{
	switch (type->typeId) // See BTNodeTypes.cpp -> Init() to match the id with the type
	{
	case 0: // Root
		break;
	case 1: // Selector
		InspectorComposite();
		break;
	case 2: // Sequence
		InspectorComposite();
		break;
	case 3: // Parallel
		break;
	case 4: // Custom Task
		InspectorTask();
		break;
	case 5: // Wait
		InspectorWait();
		break;
	}
}

void BTNode::AddDecorator(Blackboard* bb, BBVar* var)
{
	BTDecorator* dec = new BTDecorator(this, bb, var);
	decorators.push_back(dec);
}

void BTNode::RemoveDecorator(BTDecorator * dec)
{
	assert(std::find(decorators.begin(), decorators.end(), dec) != decorators.end());
	decoratorsToRemove.push_back(dec);
}

void BTNode::RemoveBBVarLinked()
{
	if (extraData)
	{
		extraData = nullptr;
	}
}

int BTNode::GetId() const
{
	return id;
}

BTNode * BTNode::GetParent() const
{
	return parent;
}

std::vector<BTNode*> BTNode::GetChilds() const
{
	return childs;
}

void BTNode::SetParent(BTNode * parent)
{
	// Note: This method does NOT add this node to its parent's child list. See AddChild(BTNode*).
	this->parent = parent;
	ReloadSubtreeId();
}

void BTNode::RemoveParent()
{
	this->parent = nullptr;
	ReloadSubtreeId();
}

void BTNode::AddChild(BTNode * child, bool pushToBack)
{
	if (std::find(childs.begin(), childs.end(), child) == childs.end())
		if (pushToBack)
			childs.push_back(child);
		else
			childs.insert(childs.begin(), child);
}

void BTNode::RemoveChild(BTNode * child)
{
	std::vector<BTNode*>::iterator found = std::find(childs.begin(), childs.end(), child);
	if (found != childs.end())
		childs.erase(found);
}

void BTNode::MoveChild(int id, bool moveUp)
{
	if (moveUp)
	{
		if (id != 0)
		{
			std::iter_swap(childs.begin() + id, childs.begin() + id - 1);
		}
	}
	else // Move down
	{
		if (id != childs.size() - 1)
		{
			std::iter_swap(childs.begin() + id, childs.begin() + id + 1);
		}
	}
}

void BTNode::ForceRoot()
{
	subtreeId = 0;
}

void BTNode::CallbackBBVarList(void * obj, const std::string & category, const std::string & item, int additionalData)
{
	BeeTEditor* editor = g_app->beetGui->beetEditor;
	if (editor->bbVarListObj)
	{
		delete editor->bbVarListObj;
		editor->bbVarListObj = nullptr;
	}
	BTNode* node = (BTNode*)obj;
	if (node)
	{
		BBVar* var = editor->btCurrent->bb->FindVar(item);
		var->nodes.push_back(node);
		node->extraData = var;
	}
}

void BTNode::Save(Data& file)
{
	Data data;
	data.AppendInt("id", id);
	data.AppendInt("parentId", (parent) ? parent->id : -1);
	data.AppendInt("type", type->typeId);

	data.AppendArray("pins");
	inputPin->Save(data);
	outputPin->Save(data);

	ImVec2 nodePosition = ne::GetNodePosition(id);
	data.AppendFloat("positionX", nodePosition.x);
	data.AppendFloat("positionY", nodePosition.y);

	data.AppendArray("childs");
	for (auto child : childs)
		child->Save(data);

	data.AppendString("name", name.data());

	data.AppendArray("decorators");
	for (auto dec : decorators)
		dec->Save(data);

	switch (type->typeId)
	{
	//case 0: // Root
	//case 1: // Selector
	//case 2: // Sequence
	//case 3: // Parallel
	case 4: // Custom Task
		if (extraData)
			data.AppendString("extraData", ((BBVar*)extraData)->name.data());
		break;
	case 5: // Wait
		data.AppendFloat("extraData", *(float*)extraData);
		break;
	}

	file.AppendArrayValue(data);
	saveFlag = true;
}

void BTNode::PreDrawSetColor()
{
	ImColor bg = ImColor(128, 128, 128, 200);
	ImColor border = highlightBorder ? ImColor(255, 0, 0, 255) : ImColor(32, 32, 32, 200);
switch (nodeColor)
{
case NC_RUNNING:
	bg = ImColor(128, 128, 50, 200);
	break;
case NC_SUCCESS:
	bg = ImColor(50, 128, 50, 200);
	break;
case NC_FAILURE:
	bg = ImColor(128, 50, 50, 200);
	break;
case NC_SUSPENDED:
	bg = ImColor(50, 50, 50, 200);
	break;
}
ne::PushStyleColor(ne::StyleColor_NodeBg, bg);
ne::PushStyleColor(ne::StyleColor_NodeBorder, border);
}

void BTNode::ReloadSubtreeId()
{
	subtreeId = parent ? parent->subtreeId : g_rnd->RandomInt();
	for (auto child : childs)
		child->ReloadSubtreeId();
}

void BTNode::InitExtraData()
{
	switch (type->typeId)
	{
	case 0: // Root
		break;
	case 1: // Selector
		break;
	case 2: // Sequence
		break;
	case 3: // Parallel
		break;
	case 4: // Custom Task
		break;
	case 5: // Wait
	{
		extraData = new char[sizeof(float)];
		float zero = 0.0f;
		memcpy(extraData, &zero, sizeof(float));
	}
	break;
	}
}

void BTNode::InspectorComposite()
{
	ImGui::Text("Execution order:");
	for (int i = 0; i < childs.size(); i++)
	{
		ImGui::PushID(childs[i]->GetId());
		ImGui::Text("%i- %s", i, childs[i]->name.data());
		ImGui::SameLine();
		if (ImGui::Button("Up"))
		{
			MoveChild(i, true);
			ImGui::PopID();
			break;
		}
		ImGui::SameLine();
		if (ImGui::Button("Down"))
		{
			MoveChild(i, false);
			ImGui::PopID();
			break;
		}
		ImGui::PopID();
	}
}

void BTNode::InspectorWait()
{
	ImGui::Text("Wait:");
	float sec = *(float*)extraData;
	if (ImGui::DragFloat("###waitNumSeconds", &sec, 1.0f, 0.0f))
	{
		memcpy(extraData, &sec, sizeof(float));
	}
	ImGui::SameLine();
	ImGui::Text(" seconds");
}

void BTNode::InspectorTask()
{
	// Additional data
	if (extraData == nullptr)
	{
		if (ImGui::Button("Add data"))
		{
			g_app->beetGui->beetEditor->ListAllBBVars(BTNode::CallbackBBVarList, this);
		}
	}
	else
	{
		BBVar* var = (BBVar*)extraData;		
		switch (var->type)
		{
			case BV_BOOL:
				ImGui::Text("Bool: %i", boost::any_cast<bool>(var->value));
				break;
			case BV_INT:
				ImGui::Text("Int: %i", boost::any_cast<int>(var->value));
				break;
			case BV_FLOAT:
				ImGui::Text("Float: %.2f", boost::any_cast<float>(var->value));
				break;
			case BV_STRING:
			{
				std::string s = boost::any_cast<std::string> (var->value);
				ImGui::Text("String: %s", s.data());
			}
				break;
			case BV_VECTOR2:
			{
				float2 val = boost::any_cast<float2>(var->value);
				ImGui::Text("Vector2: x: %.2f y: %.2f", val.x, val.y);
			}
				break;
			case BV_VECTOR3:
			{
				float3 val = boost::any_cast<float3>(var->value);
				ImGui::Text("Vector3: x: %.2f y: %.2f z: %.2f", val.x, val.y, val.z);
			}
				break;
		}

		if (ImGui::Button("Change data"))
		{
			var->nodes.erase(std::find(var->nodes.begin(), var->nodes.end(), this));
			extraData = nullptr;
			g_app->beetGui->beetEditor->ListAllBBVars(BTNode::CallbackBBVarList, this);
		}
	}
}

void BTNode::PrepToDrawTask() const
{
	BBVar* data = (BBVar*)extraData;
	if (g_app->beetGui->showNodeId)
		ImGui::Text("(%i)%s%s(%s)", id, type->icon.data(), name.data(), data->name.data());
	else
		ImGui::Text("%s%s(%s)", type->icon.data(), name.data(), data->name.data());
}

void BTNode::PrepToDrawWait() const
{
	float data = *(float*)extraData;
	if (g_app->beetGui->showNodeId)
		ImGui::Text("(%i)%s%s(%.2f)", id, type->icon.data(), name.data(), data);
	else
		ImGui::Text("%s%s(%.2f)", type->icon.data(), name.data(), data);
}
