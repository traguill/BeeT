#include "dsReturnStatus.h"
#include "dBehaviorTree.h"
#include "BTNode.h"
#include "ThirdParty/ImGui/imgui.h"

dsReturnStatus::dsReturnStatus(dBehaviorTree* bt, const Data& data) : dSample(bt, NODE_RETURNS)
{
	timestamp = data.GetFloat("timestamp");
	oldStatus = (NodeStatus)data.GetInt("oldStatus");
	newStatus = (NodeStatus)data.GetInt("newStatus");
	int nodeId = data.GetInt("nodeId");
	node = bt->FindNode(nodeId);
}

dsReturnStatus::~dsReturnStatus()
{
}

void dsReturnStatus::Print() const
{
	ImGui::Text("%s returned with:", node->name.data());
	switch (newStatus)
	{
	case NS_INVALID:
		ImGui::Text("INVALID");
		break;
	case NS_SUCCESS:
		ImGui::Text("SUCCESS");
		break;
	case NS_FAILURE:
		ImGui::Text("FAILURE");
		break;
	case NS_RUNNING:
		ImGui::Text("RUNNING");
		break;
	case NS_SUSPENDED:
		ImGui::Text("SUSPENDED");
		break;
	}
}

void dsReturnStatus::Effect()
{
	ChangeNodeColorByStatus(newStatus);
}

void dsReturnStatus::CounterEffect()
{
	ChangeNodeColorByStatus(oldStatus);
}

void dsReturnStatus::ChangeNodeColorByStatus(NodeStatus status)
{
	switch (status)
	{
	case NS_INVALID:
		node->nodeColor = NC_IDLE;
		break;
	case NS_SUCCESS:
		node->nodeColor = NC_SUCCESS;
		break;
	case NS_FAILURE:
		node->nodeColor = NC_FAILURE;
		break;
	case NS_RUNNING:
		node->nodeColor = NC_RUNNING;
		break;
	case NS_SUSPENDED:
		node->nodeColor = NC_IDLE;
		break;
	}
}
