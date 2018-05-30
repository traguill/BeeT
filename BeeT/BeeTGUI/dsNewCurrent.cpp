#include "dsNewCurrent.h"
#include "BTNode.h"
#include "dBehaviorTree.h"
#include "ThirdParty/ImGui/imgui.h"

dsNewCurrent::dsNewCurrent(dBehaviorTree* bt, const Data& data) : dSample(bt, NEW_CURRENT_NODE)
{
	timestamp = data.GetDouble("timestamp");
	int oldId = data.GetInt("oldCurrent");
	int newId = data.GetInt("newCurrent");

	oldCurrent= bt->FindNode(oldId);
	newCurrent = bt->FindNode(newId);
}

dsNewCurrent::~dsNewCurrent()
{
}

void dsNewCurrent::Print() const
{
	std::string oldName = oldCurrent ? oldCurrent->name.data() : "Root";
	std::string newName = newCurrent ? newCurrent->name.data() : "Root";
	ImGui::Text("%s -> %s", oldName.data(), newName.data());
}

void dsNewCurrent::Effect()
{
}

void dsNewCurrent::CounterEffect()
{
}
