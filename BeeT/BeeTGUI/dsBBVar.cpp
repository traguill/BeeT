#include "dsBBVar.h"
#include "ThirdParty/ImGui/imgui.h"
#include "dBehaviorTree.h"
#include "Blackboard.h"
#include <string>

using namespace std;

dsBBVar::dsBBVar(dBehaviorTree* bt, SampleType type, double timestamp) : dSample(bt, type, timestamp)
{
}

dsBBVar::~dsBBVar()
{
}

void dsBBVar::Print() const
{
	ImGui::Text("Blackboard [%s]:", name.data());

	switch (varType)
	{
	case BV_BOOL:
		if (boost::any_cast<bool>(oldValue)) ImGui::Text("From true to "); else ImGui::Text("From false to ");
		ImGui::SameLine();
		if (boost::any_cast<bool>(newValue)) ImGui::Text("true"); else ImGui::Text("false");
		break;
	case BV_INT:
		ImGui::Text("From %i to %i", boost::any_cast<int>(oldValue), boost::any_cast<int>(newValue));
		break;
	case BV_FLOAT:
		ImGui::Text("From %.2f to %.2f", boost::any_cast<float>(oldValue), boost::any_cast<float>(newValue));
		break;
	case BV_STRING:
		ImGui::Text("From %s to %s", boost::any_cast<string>(oldValue).data(), boost::any_cast<string>(newValue).data());
		break;
	}

}

void dsBBVar::Effect()
{
	BBVar* var = bt->bb->FindVar(name);
	if (var)
	{
		var->value = newValue;
	}
}

void dsBBVar::CounterEffect()
{
	BBVar* var = bt->bb->FindVar(name);
	if (var)
	{
		var->value = oldValue;
	}
}
