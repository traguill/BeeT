#include "BTNode.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "ThirdParty/NodeEditor/Source/Shared/Interop.h"
#include "Log.h"
#include "Application.h"
#include "BeeTGui.h"
#include "BehaviorTree.h"

namespace ne = ax::NodeEditor;

BTNode::BTNode(int id, int sourcePinId, int targetPinId, int typeId, BehaviorTree* bt, BTNode* parent) : id(id), bt(bt), parent(parent)
{
	// Temporal way to set the node's name
	char buf[100];
	snprintf(buf, sizeof(buf), "Node %i", id);
	name = buf;
	// --------------------------------------------	
	type = g_app->beetGui->btNodeTypes->GetTypeById(typeId);

	inputPin = new BTPin(targetPinId, ne::PinKind::Target, this);
	outputPin = new BTPin(sourcePinId, ne::PinKind::Source, this);
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
	comment = data.GetString("comment");
}

BTNode::~BTNode()
{
	delete inputPin;
	delete outputPin;
}

std::vector<BTLink*> BTNode::ClearLinks()
{
	std::vector<BTLink*> linkList;
	for (std::vector<BTLink*>::iterator link = inputPin->links.begin(); link != inputPin->links.end(); ++link)
	{
		linkList.push_back(*link);
		(*link)->CleanUp(false);
	}
	for (std::vector<BTLink*>::iterator link = outputPin->links.begin(); link != outputPin->links.end(); ++link)
	{
		linkList.push_back(*link);
		(*link)->CleanUp(true);
	}

	return linkList;
}

void BTNode::PrepareToDraw()
{
	ne::BeginNode(id);
	ImGui::BeginVertical(id);

	// Input ---------------------------------
	ImGui::BeginHorizontal("inputs");
	ImGui::Spring(0, padding * 2);

	if (bool nodeHasInput = true)
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

	ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
	ImGui::Dummy(ImVec2(160, 0));

	ImGui::Spring(1);
	ImGui::Text("[%s]", type->name.data());
	ImGui::Spring(1);
	ImGui::Text("%s", name.data());
	ImGui::Spring(1);

	ImGui::EndVertical(); // 'content'

	contentRect = ImGui_GetItemRect();
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

int BTNode::GetId() const
{
	return id;
}

std::string BTNode::GetName() const
{
	return name;
}

BTNode * BTNode::GetParent() const
{
	return parent;
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

	data.AppendString("name", name.data());
	data.AppendString("comment", comment.data());

	file.AppendArrayValue(data);
	saveFlag = true;
}
