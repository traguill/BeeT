#include "dsBTEnd.h"
#include "dBehaviorTree.h"
#include "ThirdParty/ImGui/imgui.h"
#include <stack>

dsBTEnd::dsBTEnd(dBehaviorTree * bt, const Data & data) : dSample(bt, BT_END)
{
	timestamp = data.GetFloat("timestamp");
}

dsBTEnd::~dsBTEnd()
{
	for (auto node : treeNodes)
		delete node;
}

void dsBTEnd::Print() const
{
	ImGui::Text("--- BT END ---");
}

void dsBTEnd::Effect()
{
	if (treeNodes.size() != 0)
	{
		for (auto item : treeNodes)
		{
			item->prevColor = item->node->nodeColor;
			item->node->nodeColor = NC_IDLE;
			item->node->highlightBorder = false;
		}
	}
	else
	{
		// Init
		BTNode* node = bt->GetRootNode();
		std::stack<BTNode*> stack;
		stack.push(node);
		while (stack.size() > 0)
		{
			node = stack.top();
			stack.pop();
			BTNodePrevEnd* item = new BTNodePrevEnd();
			item->node = node;
			item->prevColor = node->nodeColor;
			node->nodeColor = NC_IDLE;
			node->highlightBorder = false;
			treeNodes.push_back(item);
			std::vector<BTNode*> childs = node->GetChilds();
			for (auto child : childs)
				stack.push(child);
		}
	}	
}

void dsBTEnd::CounterEffect()
{
	for (auto item : treeNodes)
		item->node->nodeColor = item->prevColor;
}
