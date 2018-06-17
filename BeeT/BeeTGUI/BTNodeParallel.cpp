#include "BTNodeParallel.h"
#include "BTDecorator.h"
#include "BehaviorTree.h"
#include "Application.h"
#include "BeeTGui.h"

#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "ThirdParty/NodeEditor/Source/Shared/Interop.h"

namespace ne = ax::NodeEditor;

BTNodeParallel::BTNodeParallel(int id, int sourcePinId, int targetPinId, int targetPin2Id, BehaviorTree * bt, BTNode * parent)
	: BTNode(id, sourcePinId, targetPin2Id, 3, bt, parent)
{
	simplePin = new BTPin(targetPinId, ne::PinKind::Source, this, true);
}

BTNodeParallel::BTNodeParallel(BehaviorTree * bt, Data & data)
{
	this->bt = bt;
	id = data.GetInt("id");
	int parentId = data.GetInt("parentId");
	parent = parentId > 0 ? bt->FindNode(parentId) : nullptr;
	type = g_app->beetGui->btNodeTypes->GetTypeById(data.GetInt("type"));

	inputPin = new BTPin(this, data.GetArray("pins", 0));
	outputPin = new BTPin(this, data.GetArray("pins", 1));
	simplePin = new BTPin(this, data.GetArray("pins", 2));

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
		//case 4: // Custom Task
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

BTNodeParallel::~BTNodeParallel()
{
	delete simplePin;
}

void BTNodeParallel::Save(Data & file)
{
	Data data;
	data.AppendInt("id", id);
	data.AppendInt("parentId", (parent) ? parent->GetId() : -1);
	data.AppendInt("type", type->typeId);

	data.AppendArray("pins");
	inputPin->Save(data);
	outputPin->Save(data);
	simplePin->Save(data);

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
		//case 4: // Custom Task
	case 5: // Wait
		data.AppendFloat("extraData", *(float*)extraData);
		break;
	}

	file.AppendArrayValue(data);
	saveFlag = true;
}

void BTNodeParallel::InspectorInfo()
{
	ImGui::Text("Choose type:"); // TODO: set diferent break types
	ImGui::Text("Abort when main node fails");
	ImGui::Text("Wait until subtree finishes");

	ImGui::Text("Execution order:");
	for (int i = 0; i < childs.size(); i++)
	{
		ImGui::PushID(childs[i]->GetId());
		ImGui::Text("%i- %s", i, childs[i]->name.data());
		ImGui::PopID();
	}
}

void BTNodeParallel::PrepareToDraw()
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
	if (g_app->beetGui->showNodeId)
		ImGui::Text("(%i)%s%s", id, type->icon.data(), name.data());
	else
		ImGui::Text("%s%s", type->icon.data(), name.data());
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


	ImGui::Dummy(ImVec2(0, padding));
	ImGui::Spring(1, 0);
	outputsRect = simpleOutputsRect = ImGui_GetItemRect();

	outputsRect.x += outputsRect.w * 0.25f;
	outputsRect.w *= 0.75f;

	simpleOutputsRect.w *= 0.2f;

	ne::PushStyleVar(ne::StyleVar_PinCorners, 3);
	
	ne::BeginPin(outputPin->id, ne::PinKind::Source);
	ne::PinPivotRect(to_imvec(outputsRect.top_left()), to_imvec(outputsRect.bottom_right()));
	ne::PinRect(to_imvec(outputsRect.top_left()), to_imvec(outputsRect.bottom_right()));
	ne::EndPin();

	ne::BeginPin(simplePin->id, ne::PinKind::Source); // TODO: Own output
	ne::PinPivotRect(to_imvec(simpleOutputsRect.top_left()), to_imvec(simpleOutputsRect.bottom_right()));
	ne::PinRect(to_imvec(simpleOutputsRect.top_left()), to_imvec(simpleOutputsRect.bottom_right()));
	ne::EndPin();

	ne::PopStyleVar();
	

	ImGui::Spring(0, padding * 2);
	ImGui::EndHorizontal(); // 'outputs'
	// -------------------------------------------

	ImGui::EndVertical(); // nodeId
	ne::EndNode();
}

std::vector<BTLink*> BTNodeParallel::GetAllLinks()
{
	std::vector<BTLink*> linkList;
	for (std::vector<BTLink*>::iterator link = inputPin->links.begin(); link != inputPin->links.end(); ++link)
		linkList.push_back(*link);

	for (std::vector<BTLink*>::iterator link = outputPin->links.begin(); link != outputPin->links.end(); ++link)
		linkList.push_back(*link);

	for (std::vector<BTLink*>::iterator link = simplePin->links.begin(); link != simplePin->links.end(); ++link)
		linkList.push_back(*link);

	return linkList;
}
