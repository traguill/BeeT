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
	var->value = NULL;

	variables.push_back(var);
}

void Blackboard::SetLastTypeUsed(BBVarType type)
{
	lastTypeUsed = type;
}
