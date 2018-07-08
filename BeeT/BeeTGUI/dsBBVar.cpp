#include "dsBBVar.h"
#include "ThirdParty/ImGui/imgui.h"
#include "dBehaviorTree.h"
#include "Blackboard.h"
#include <string>

using namespace std;

dsBBVar::dsBBVar(dBehaviorTree* bt, const Data& data) : dSample(bt, BBVAR_CHANGED)
{
	timestamp = data.GetFloat("timestamp");
	name = data.GetString("name");
	varType = (BBVarType)data.GetInt("varType");

	switch (varType)
	{
	case BV_BOOL:
		oldValue = data.GetBool("oldValue");
		newValue = data.GetBool("newValue");
		break;
	case BV_INT:
		oldValue = data.GetInt("oldValue");
		newValue = data.GetInt("newValue");
		break;
	case BV_FLOAT:
		oldValue = data.GetFloat("oldValue");
		newValue = data.GetFloat("newValue");
		break;
	case BV_STRING:
		oldValue = string(data.GetString("oldValue"));
		newValue = string(data.GetString("newValue"));
		break;
	case BV_VECTOR2:
		oldValue = data.GetFloat2("oldValue");
		newValue = data.GetFloat2("newValue");
		break;
	case BV_VECTOR3:
		oldValue = data.GetFloat3("oldValue");
		newValue = data.GetFloat3("newValue");
		break;
	}
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
	case BV_VECTOR2:
	{
		float2 vold = boost::any_cast<float2>(oldValue);
		float2 vnew = boost::any_cast<float2>(newValue);
		ImGui::Text("From (%.2f, %.2f) to (%.2f, %.2f)", vold.x, vold.y, vnew.x, vnew.y);
	}
		break;
	case BV_VECTOR3:
	{
		float3 vold = boost::any_cast<float3>(oldValue);
		float3 vnew = boost::any_cast<float3>(newValue);
		ImGui::Text("From (%.2f, %.2f, %.2f) to (%.2f, %.2f, %.2f)", vold.x, vold.y, vold.z, vnew.x, vnew.y, vnew.z);
	}
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
