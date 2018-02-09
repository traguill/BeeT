#include "BTNode.h"
#include "BTLink.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "ThirdParty/NodeEditor/Source/Shared/Interop.h"
#include "Log.h"

namespace ne = ax::NodeEditor;

BTNode::BTNode(int id, int sourcePinId, int targetPinId, BTNodeType type, BTNode* parent, BTLink* inputLink) : id(id), sourcePinId(sourcePinId), targetPinId(targetPinId), type(type), parent(parent), inputLink(inputLink)
{
	LOGI("Node id: %i sourcePin: %i targetPin: %i", id, sourcePinId, targetPinId);
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
		ne::BeginPin(targetPinId, ne::PinKind::Target);
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
	ImGui::TextUnformatted("Node's Name");
	ImGui::Spring(1);

	ImGui::EndVertical(); // 'content'

	contentRect = ImGui_GetItemRect();
	ImGui::Spring(1, padding);
	ImGui::EndHorizontal(); // 'content_frame'
	// -------------------------------------------

	// Output ------------------------------------
	ImGui::BeginHorizontal("outputs");
	ImGui::Spring(0, padding * 2);


	// For simplicity, this node has no outputs. Same code as inputs
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
