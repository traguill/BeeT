#include "BehaviorTree.h"
#include "BTNode.h"
#include "BTLink.h"
#include "BTNodeTypes.h"
#include "Log.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"
#include "ThirdParty/NodeEditor/Source/Shared/Interop.h"

namespace ne = ax::NodeEditor;
using namespace std;

BehaviorTree::BehaviorTree()
{}

BehaviorTree::~BehaviorTree()
{
	for (auto node : nodesList)
	{
		delete node.second;
	}
}

void BehaviorTree::AddNode(float posX, float posY)
{
	int id = GetNextId();
	BTNode* node = new BTNode(id, GetNextId(), GetNextId(), BTNodeType::ACTION);

	nodesList.insert(pair<int, BTNode*>(id, node));

	ne::SetNodePosition(node->GetId(), ImVec2(posX, posY));
	LOGI("Node %i created", id);
}

void BehaviorTree::Draw()
{
	// Style Colors
	ne::PushStyleColor(ne::StyleColor_NodeBg, ImColor(128, 128, 128, 200));
	ne::PushStyleColor(ne::StyleColor_NodeBorder, ImColor(32, 32, 32, 200));
	ne::PushStyleColor(ne::StyleColor_PinRect, ImColor(60, 180, 255, 150));
	ne::PushStyleColor(ne::StyleColor_PinRectBorder, ImColor(60, 180, 255, 150));

	// Style Vars
	ne::PushStyleVar(ne::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
	ne::PushStyleVar(ne::StyleVar_NodeRounding, rounding);
	ne::PushStyleVar(ne::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
	ne::PushStyleVar(ne::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
	ne::PushStyleVar(ne::StyleVar_LinkStrength, 0.0f);
	ne::PushStyleVar(ne::StyleVar_PinBorderWidth, 1.0f);
	ne::PushStyleVar(ne::StyleVar_PinRadius, 5.0f);

	// Prepare the nodes to draw
	for (auto node : nodesList)
	{
		node.second->PrepareToDraw();
	}

	ne::PopStyleVar(7);
	ne::PopStyleColor(4);

	// Actually draw the nodes
	int inputAlpha = 200;
	const ImVec4 pinBackground = ne::GetStyle().Colors[ne::StyleColor_NodeBg];
	int outputAlpha = 200;

	for (auto node : nodesList)
	{
		ImDrawList* drawList = ne::GetNodeBackgroundDrawList(node.second->GetId());

		// Draw Input
		drawList->AddRectFilled(to_imvec(node.second->inputsRect.top_left()) + ImVec2(0, 1), to_imvec(node.second->inputsRect.bottom_right()), IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, 12);

		ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
		drawList->AddRect(to_imvec(node.second->inputsRect.top_left()) + ImVec2(0, 1), to_imvec(node.second->inputsRect.bottom_right()), IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), inputAlpha), 4.0f, 12);
		ImGui::PopStyleVar();

		// Draw Output
		drawList->AddRectFilled(to_imvec(node.second->outputsRect.top_left()), to_imvec(node.second->outputsRect.bottom_right()) - ImVec2(0, 1), IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, 3);

		ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
		drawList->AddRect(to_imvec(node.second->outputsRect.top_left()), to_imvec(node.second->outputsRect.bottom_right()) - ImVec2(0, 1), IM_COL32((int)(255 * pinBackground.x), (int)(255 * pinBackground.y), (int)(255 * pinBackground.z), outputAlpha), 4.0f, 3);
		ImGui::PopStyleVar();

		// Draw Content
		drawList->AddRectFilled(to_imvec(node.second->contentRect.top_left()), to_imvec(node.second->contentRect.bottom_right()), IM_COL32(24, 64, 128, 200), 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_AntiAliasFringeScale, 1.0f);
		drawList->AddRect(to_imvec(node.second->contentRect.top_left()), to_imvec(node.second->contentRect.bottom_right()), IM_COL32(48, 128, 255, 100), 0.0f);
		ImGui::PopStyleVar();
	}
}

int BehaviorTree::GetNextId()
{
	return nextId++;
}