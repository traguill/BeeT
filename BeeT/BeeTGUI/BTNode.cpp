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

namespace ne = ax::NodeEditor;

BTNode::BTNode(int id, int sourcePinId, int targetPinId, int typeId, BehaviorTree* bt, BTNode* parent) : id(id), bt(bt), parent(parent)
{
	type = g_app->beetGui->btNodeTypes->GetTypeById(typeId);
	if(type)
		name = type->name;

	inputPin = new BTPin(targetPinId, ne::PinKind::Target, this);
	outputPin = new BTPin(sourcePinId, ne::PinKind::Source, this);
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

	ReloadSubtreeId();
}

BTNode::~BTNode()
{
	for (auto dec : decorators)
		delete dec;
	delete inputPin;
	delete outputPin;
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
	ImGui::Text("%s", name.data());
	ImGui::Spring(1);
	ImGui::Text("[%s]", type->name.data());
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

void BTNode::AddChild(BTNode * child)
{
	if(std::find(childs.begin(), childs.end(), child) == childs.end())
		childs.push_back(child);
}

void BTNode::RemoveChild(BTNode * child)
{
	std::vector<BTNode*>::iterator found = std::find(childs.begin(), childs.end(), child);
	if (found != childs.end())
		childs.erase(found);
}

void BTNode::ForceRoot()
{
	subtreeId = 0;
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

	file.AppendArrayValue(data);
	saveFlag = true;
}

void BTNode::ReloadSubtreeId()
{
	subtreeId = parent ? parent->subtreeId : g_rnd->RandomInt();
	for (auto child : childs)
		child->ReloadSubtreeId();
}
