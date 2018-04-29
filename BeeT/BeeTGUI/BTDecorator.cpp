#include "BTDecorator.h"
#include "ThirdParty/NodeEditor/Source/Shared/Interop.h"
#include "Random.h"
#include "Blackboard.h"

BTDecorator::BTDecorator(Blackboard* bb, BBVar* var) : bb(bb), var(var)
{
	uid = (int)g_rnd->RandomInt();
	switch (var->type)
	{
	case BV_BOOL:
		var2 = false;
		break;
	case BV_INT:
		var2 = 0;
		break;
	case BV_FLOAT:
		var2 = 0.0f;
		break;
	case BV_STRING:
		var2 = std::string("");
		break;
	}
}

BTDecorator::BTDecorator(Blackboard * bb, Data & data) : bb(bb)
{
	uid = data.GetInt("uid");
	std::string varName = data.GetString("var");
	if (varName.length() > 0)
		var = bb->FindVar(varName);
	else
		var = nullptr;
	option = data.GetInt("option");
	if (var)
	{
		switch (var->type)
		{
		case BV_BOOL:
			break;
		case BV_INT:
			var2 = data.GetInt("var2");
			break;
		case BV_FLOAT:
			var2 = data.GetFloat("var2");
			break;
		case BV_STRING:
			var2 = std::string(data.GetString("var2"));
			break;
		}
	}
}

BTDecorator::~BTDecorator()
{
}

void BTDecorator::PrepareToDraw()
{
	if (var == nullptr)
		return;

	ImGui::PushID(uid);
	ImGui::BeginVertical("content", ImVec2(0.0f, 0.0f));
	ImGui::Dummy(ImVec2(160, 0));

	ImGui::Spring(1);
	ImGui::Text("Blackboard: Condition");
	ImGui::Spring(1);
	switch (var->type)
	{
	case BV_BOOL:
		PrepDrawBool();
		break;
	case BV_INT:
		PrepDrawInt();
		break;
	case BV_FLOAT:
		PrepDrawFloat();
		break;
	case BV_STRING:
		PrepDrawString();
		break;
	}
	ImGui::Spring(1);

	ImGui::EndVertical(); // 'content'
	ImGui::PopID();

	contentRect = ImGui_GetItemRect();
}

void BTDecorator::InspectorInfo()
{
	if (var == nullptr)
		return;
	PrintType();
	ImGui::SameLine();
	ImGui::Text("%s", var->name.data());
	//ImGui::SameLine();
	switch (var->type)
	{
	case BV_BOOL:
		TypeBoolOptions();
		break;
	case BV_INT:
		TypeNumberOptions(0);
		break;
	case BV_FLOAT:
		TypeNumberOptions(1);
		break;
	case BV_STRING:
		TypeStringOptions();
		break;
	}

	ImGui::Spacing();
}

void BTDecorator::Save(Data & file)
{
	Data data;

	data.AppendInt("uid", uid);
	if (var)
		data.AppendString("var", var->name.data());
	else
		data.AppendString("var", "");
	data.AppendInt("option", option);

	if (var)
	{
		switch (var->type)
		{
		case BV_BOOL:
			break;
		case BV_INT:
			data.AppendInt("var2", boost::any_cast<int>(var2));
			break;
		case BV_FLOAT:
			data.AppendFloat("var2", boost::any_cast<float>(var2));
			break;
		case BV_STRING:
			data.AppendString("var2", boost::any_cast<std::string>(var2).data());
			break;
		}
	}

	file.AppendArrayValue(data);
}

void BTDecorator::PrintType() const
{
	switch (var->type)
	{
	case BV_BOOL:
		ImGui::Text("bool   ");
		break;
	case BV_INT:
		ImGui::Text("int    ");
		break;
	case BV_FLOAT:
		ImGui::Text("float  ");
		break;
	case BV_STRING:
		ImGui::Text("string ");
		break;
	}
}

void BTDecorator::PrepDrawBool()
{
	if (option == (int)DecBoolOpt::IS_FALSE)
	{
		ImGui::Text("%s is False", var->name.data());
	}
	else
	{
		ImGui::Text("%s is True", var->name.data());
	}
}

void BTDecorator::PrepDrawInt()
{
	switch ((DecNumberOpt)option)
	{
	case IS_EQUAL:
		ImGui::Text("%s is Equal to", var->name.data());
		break;
	case IS_NOT_EQUAL:
		ImGui::Text("%s is Not Equal to", var->name.data());
		break;
	case IS_GREATER:
		ImGui::Text("%s is Greater than", var->name.data());
		break;
	case IS_LESS:
		ImGui::Text("%s is less than", var->name.data());
		break;
	case IS_EQUAL_OR_GREATER:
		ImGui::Text("%s is Equal or Greater than", var->name.data());
		break;
	case IS_EQUAL_OR_LESS:
		ImGui::Text("%s is Equal or Less than", var->name.data());
		break;
	}
}

void BTDecorator::PrepDrawFloat()
{
	switch ((DecNumberOpt)option)
	{
	case IS_EQUAL:
		ImGui::Text("%s is Equal to", var->name.data());
		break;
	case IS_NOT_EQUAL:
		ImGui::Text("%s is Not Equal to", var->name.data());
		break;
	case IS_GREATER:
		ImGui::Text("%s is Greater than", var->name.data());
		break;
	case IS_LESS:
		ImGui::Text("%s is less than", var->name.data());
		break;
	case IS_EQUAL_OR_GREATER:
		ImGui::Text("%s is Equal or Greater than", var->name.data());
		break;
	case IS_EQUAL_OR_LESS:
		ImGui::Text("%s is Equal or Less than", var->name.data());
		break;
	}
}

void BTDecorator::PrepDrawString()
{
	switch ((DecStringOpt)option)
	{
	case S_IS_EQUAL:
		ImGui::Text("%s is Equal to", var->name.data());
		break;
	case S_IS_NOT_EQUAL:
		ImGui::Text("%s is Not Equal to", var->name.data());
		break;
	case CONTAINS:
		ImGui::Text("%s Contains", var->name.data());
		break;
	case NOT_CONTAINS:
		ImGui::Text("%s Not Contains", var->name.data());
		break;
	}
}

void BTDecorator::TypeBoolOptions()
{
	const char* optText = (option == (int)DecBoolOpt::IS_FALSE) ? "IS FALSE" : "IS TRUE";

	if (ImGui::Selectable(optText))
	{
		ImGui::OpenPopup("DecBoolOptions");
	}
	
	if (ImGui::BeginPopup("DecBoolOptions"))
	{
		if (ImGui::MenuItem("IS FALSE"))
			option = (int)DecBoolOpt::IS_FALSE;
		if (ImGui::MenuItem("IS TRUE"))
			option = (int)DecBoolOpt::IS_TRUE;
		ImGui::EndPopup(); 
	}
}

void BTDecorator::TypeNumberOptions(int numType)
{
	const char* optText;
	switch ((DecNumberOpt)option)
	{
		case IS_EQUAL:
			optText = "IS EQUAL to ";
			break;
		case IS_NOT_EQUAL:
			optText = "IS NOT EQUAL to ";
			break;
		case IS_GREATER:
			optText = "IS GREATER than ";
			break;
		case IS_LESS:
			optText = "IS LESS than ";
			break;
		case IS_EQUAL_OR_GREATER:
			optText = "IS EQUAL OR GREATER than ";
			break;
		case IS_EQUAL_OR_LESS:
			optText = "IS EQUAL OR LESS than ";
			break;
		default:
			optText = "This should not be here";
			break;
	}

	if (ImGui::Selectable(optText))
	{
		ImGui::OpenPopup("DecNumOptions");
	}

	if (ImGui::BeginPopup("DecNumOptions"))
	{
		if (ImGui::MenuItem("IS EQUAL")) option = (int)DecNumberOpt::IS_EQUAL;
		if (ImGui::MenuItem("IS NOT EQUAL")) option = (int)DecNumberOpt::IS_NOT_EQUAL;
		if (ImGui::MenuItem("IS GREATER")) option = (int)DecNumberOpt::IS_GREATER;
		if (ImGui::MenuItem("IS LESS")) option = (int)DecNumberOpt::IS_LESS;
		if (ImGui::MenuItem("IS EQUAL OR GREATER")) option = (int)DecNumberOpt::IS_EQUAL_OR_GREATER;
		if (ImGui::MenuItem("IS EQUAL OR LESS")) option = (int)DecNumberOpt::IS_EQUAL_OR_LESS;

		ImGui::EndPopup();
	}

	if (numType == 0) //Int
	{
		//ImGui::SameLine();
		int tmp = boost::any_cast<int>(var2);
		if (ImGui::DragInt("###DecIntCompare", &tmp))
		{
			var2 = tmp;
		}
	}
	if (numType == 1) // Float
	{
		//ImGui::SameLine();
		float tmp = boost::any_cast<float>(var2);
		if (ImGui::DragFloat("###DecFloatCompare", &tmp))
		{
			var2 = tmp;
		}
	}
}

void BTDecorator::TypeStringOptions()
{
	const char* optText;

	switch ((DecStringOpt)option)
	{
	case S_IS_EQUAL:
		optText = "Is Equal to ";
		break;
	case S_IS_NOT_EQUAL:
		optText = "Is Not Equal to";
		break;
	case CONTAINS:
		optText = "Contains";
		break;
	case NOT_CONTAINS:
		optText = "Not Contains";
		break;
	default:
		optText = "This should not be here";
		break;
	}

	if (ImGui::Selectable(optText))
	{
		ImGui::OpenPopup("DecStringOptions");
	}

	if (ImGui::BeginPopup("DecStringOptions"))
	{
		if (ImGui::MenuItem("IS EQUAL")) option = (int)DecStringOpt::S_IS_EQUAL;
		if (ImGui::MenuItem("IS NOT EQUAL")) option = (int)DecStringOpt::S_IS_NOT_EQUAL;
		if (ImGui::MenuItem("CONTAINS")) option = (int)DecStringOpt::CONTAINS;
		if (ImGui::MenuItem("NOT CONTAINS")) option = (int)DecStringOpt::NOT_CONTAINS;
		ImGui::EndPopup();
	}

	//ImGui::SameLine();
	std::string valString = boost::any_cast<std::string>(var2);
	char varNameTmp[_MAX_PATH];
	strcpy_s(varNameTmp, _MAX_PATH, valString.data());
	if (ImGui::InputText("###DecStringCompare", varNameTmp, _MAX_PATH))
	{
		var2 = std::string(varNameTmp);
	}
}