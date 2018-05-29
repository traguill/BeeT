#include "dsBBVar.h"
#include "ThirdParty/ImGui/imgui.h"

dsBBVar::dsBBVar(SampleType type, double timestamp) : dSample(type, timestamp)
{
}

dsBBVar::~dsBBVar()
{
}

void dsBBVar::Print() const
{
	ImGui::Text("Blackboard variable [%s] changed", name.data());

	switch (varType)
	{
	case BV_BOOL:
		if (*(bool*)oldValue) ImGui::Text("From true to "); else ImGui::Text("From false to ");
		ImGui::SameLine();
		if (*(bool*)newValue) ImGui::Text("true"); else ImGui::Text("false");
		break;
	case BV_INT:
		ImGui::Text("From %i to %i", *(int*)oldValue, *(int*)newValue);
		break;
	case BV_FLOAT:
		ImGui::Text("From %.2f to %.2f", *(float*)oldValue, *(float*)newValue);
		break;
	case BV_STRING:
		break;
	}

}
