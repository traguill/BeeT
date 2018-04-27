#include "Blackboard.h"

using namespace std;

Blackboard::Blackboard()
{
}

Blackboard::~Blackboard()
{
	for (auto var : variables)
	{
		if (var)
			delete var;
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
	default:
		var->value = NULL;
		break;
	}
}
