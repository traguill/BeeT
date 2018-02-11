#include "BTNode.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "ThirdParty/NodeEditor/Source/Shared/Interop.h"
#include "Log.h"
#include "Application.h"
#include "BeeTGui.h"

namespace ne = ax::NodeEditor;

BTNode::BTNode(int id, int sourcePinId, int targetPinId, int typeId, BTNode* parent) : id(id), parent(parent)
{
	// Temporal way to set the node's name
	char buf[100];
	snprintf(buf, sizeof(buf), "Node %i", id);
	name = buf;
	// --------------------------------------------	
	type = g_app->beetGui->btNodeTypes->GetTypeById(typeId);

	inputPin.id = targetPinId;
	inputPin.kind = ne::PinKind::Target;
	inputPin.node = this;

	outputPin.id = sourcePinId;
	outputPin.kind = ne::PinKind::Source;
	outputPin.node = this;
}

BTNode::~BTNode()
{}

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
		ne::BeginPin(inputPin.id, ne::PinKind::Target);
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
		ne::BeginPin(outputPin.id, ne::PinKind::Source);
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

std::string BTNode::GetTypeName() const
{
	return type->name;
}

bool BTNode::IsInputLinkAvailable() const
{
	return (numInputLinks == 0) ? true : false;
}

bool BTNode::IsOutputLinkAvailable() const
{
	return (type->maxOutputs > numOutputLinks || type->maxOutputs == -1) ? true : false;
}

void BTNode::CreateInputLink()
{
	if (numInputLinks == 0)
		++numInputLinks;
	else
		LOGW("Cannot create another input link! The node (%i) %s already have a input link connection", id, name.data());
}

void BTNode::CreateOutputLink()
{
	if (type->maxOutputs > numOutputLinks || type->maxOutputs == -1)
		++numOutputLinks;
	else
		LOGW("Cannot create another output link! The node (%i) %s already have the maximum output link connections", id, name.data());
}
