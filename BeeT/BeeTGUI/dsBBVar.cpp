#include "dsBBVar.h"
#include "ThirdParty/ImGui/imgui.h"
#include "dBehaviorTree.h"
#include "Blackboard.h"

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

void dsBBVar::Effect()
{
	BBVar* var = bt->bb->FindVar(name);
	if (var)
	{
		switch (varType)
		{
		case BV_BOOL:
			var->value = *(bool*)newValue;
			break;
		case BV_INT:
			var->value = *(int*)newValue;
			break;
		case BV_FLOAT:
			var->value = *(float*)newValue;
			break;
		case BV_STRING:
			break;
		}
	}
}

void dsBBVar::CounterEffect()
{
	BBVar* var = bt->bb->FindVar(name);
	if (var)
	{
		switch (varType)
		{
		case BV_BOOL:
			var->value = *(bool*)oldValue;
			break;
		case BV_INT:
			var->value = *(int*)oldValue;
			break;
		case BV_FLOAT:
			var->value = *(float*)oldValue;
			break;
		case BV_STRING:
			break;
		}
	}
}
