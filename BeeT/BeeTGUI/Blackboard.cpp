#include "Blackboard.h"
#include "BTDecorator.h"
#include "BasicStructs.h"

using namespace std;

Blackboard::Blackboard()
{
}

Blackboard::Blackboard(Data & data)
{
	dummyId = data.GetInt("dummyId");
	int bbSize = data.GetArraySize("blackboard");
	for (int i = 0; i < bbSize; ++i)
	{
		Data varData = data.GetArray("blackboard", i);
		BBVar* var = new BBVar();
		var->name = varData.GetString("name");
		var->type = (BBVarType)varData.GetInt("type");
		switch (var->type)
		{
		case BV_BOOL:
			var->value = varData.GetBool("value");
			break;
		case BV_INT:
			var->value = varData.GetInt("value");
			break;
		case BV_FLOAT:
			var->value = varData.GetFloat("value");
			break;
		case BV_STRING:
			var->value = string(varData.GetString("value"));
			break;
		case BV_VECTOR2:
			var->value = varData.GetFloat2("value");
			break;
		case BV_VECTOR3:
			var->value = varData.GetFloat3("value");
			break;
		default:
			var->value = NULL;
			break;
		}

		variables.push_back(var);
	}
}

Blackboard::~Blackboard()
{
	for (auto var : variables)
	{
		if (var)
			delete var;
	}
}

void Blackboard::Serialize(Data & data) const
{
	data.AppendInt("dummyId", dummyId);
	data.AppendArray("blackboard");
	for (auto var : variables)
	{
		Data varData;

		varData.AppendInt("type", (int)var->type);
		varData.AppendString("name", var->name.data());
		switch (var->type)
		{
		case BV_BOOL:
			varData.AppendBool("value", boost::any_cast<bool>(var->value));
			break;
		case BV_INT:
			varData.AppendInt("value", boost::any_cast<int>(var->value));
			break;
		case BV_FLOAT:
			varData.AppendFloat("value", boost::any_cast<float>(var->value));
			break;
		case BV_STRING:
			varData.AppendString("value", boost::any_cast<string>(var->value).data());
			break;
		case BV_VECTOR2:
			varData.AppendFloat2("value", boost::any_cast<float2>(var->value));
			break;
		case BV_VECTOR3:
			varData.AppendFloat3("value", boost::any_cast<float3>(var->value));
			break;
		}

		data.AppendArrayValue(varData);
	}
}

void Blackboard::CreateDummyVar()
{
	string dummyText = "BBVar";
	dummyText.append(to_string(dummyId));
	dummyId++;
	
	BBVar* var = new BBVar();
	var->name = dummyText;
	var->type = lastTypeUsed;
	SetDefaultTypeValue(var);

	variables.push_back(var);
}

void Blackboard::RemoveVar(int id)
{
	assert(id < variables.size());
	BBVar* var = variables[id];
	for (auto dec : var->decorators)
	{
		dec->Remove(true);
	}
	delete var;
	variables.erase(variables.begin() + id);
}

void Blackboard::ChangeVarType(int varId, BBVarType type)
{
	if (varId < variables.size())
	{
		BBVar* var = variables[varId];
		var->type = type;
		SetDefaultTypeValue(var);
		lastTypeUsed = type;
	}
}

BBVar * Blackboard::FindVar(const std::string & name)
{
	for (auto var : variables)
		if (var->name.compare(name) == 0)
			return var;

	return nullptr;
}

bool Blackboard::IsVarNameUnique(const std::string & name) const
{
	for (auto var : variables)
	{
		if (var->name.compare(name) == 0)
			return false;
	}
	return true;
}

void Blackboard::SetDefaultTypeValue(BBVar * var)
{
	switch (var->type)
	{
	case BV_BOOL:
		var->value = false;
		break;
	case BV_INT:
		var->value = 0;
		break;
	case BV_FLOAT:
		var->value = 0.0f;
		break;
	case BV_STRING:
		var->value = string("");
		break;
	case BV_VECTOR2:
		var->value = float2();
		break;
	case BV_VECTOR3:
		var->value = float3();
		break;
	default:
		var->value = NULL;
		break;
	}
}
