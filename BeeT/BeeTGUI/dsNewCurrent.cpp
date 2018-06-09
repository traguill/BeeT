#include "dsNewCurrent.h"
#include "BTNode.h"
#include "dBehaviorTree.h"
#include "ThirdParty/ImGui/imgui.h"

dsNewCurrent::dsNewCurrent(dBehaviorTree* bt, const Data& data) : dSample(bt, NEW_CURRENT_NODE)
{
	timestamp = data.GetFloat("timestamp");
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
	std::string newName = newCurrent ? newCurrent->name.data() : "Root";
	ImGui::Text("-- %s --", newName.data());
}

void dsNewCurrent::Effect()
{
	if(newCurrent)
		newCurrent->highlightBorder = true;
	if(oldCurrent)
		oldCurrent->highlightBorder = false;
}

void dsNewCurrent::CounterEffect()
{
	if(oldCurrent)
		oldCurrent->highlightBorder = true;
	if(newCurrent)
		newCurrent->highlightBorder = false;
}
