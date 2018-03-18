#include "BTNodeTypes.h"
#include "Log.h"
#include "ItemList.h"

using namespace std;

NodeType::NodeType()
{}

NodeType::NodeType(int typeId, const std::string& category, const std::string& name, int maxOutputs) : typeId(typeId), category(category), name(name), maxOutputs(maxOutputs)
{}

BTNodeTypes::BTNodeTypes()
{
}

BTNodeTypes::~BTNodeTypes()
{
	if (listObject)
		delete listObject;
}

void BTNodeTypes::Init()
{
	listObject = new ListObject(); // Container to display all types in the ItemList widget

	// Default node types
	InsertType("", "Root", 1);
	
	InsertType("Composites", "Selector", -1);
	InsertType("Composites", "Sequence", -1);
	InsertType("Composites", "Parallel", -1);

	InsertType("Tasks", "Custom Task", 0);
}

NodeType * BTNodeTypes::GetTypeById(int id)
{
	if ((unsigned int)id >= typesList.size() || id < 0)
	{
		LOGW("Couldn't find node type with id: %", id);
		return nullptr;
	}
	
	return &typesList[id];
}

int BTNodeTypes::GetNodeTypeId(const std::string & category, const std::string & name) const
{
	for (int i = 0; i < typesList.size(); i++)
	{
		if (typesList[i].category.compare(category) == 0)
			if (typesList[i].name.compare(name) == 0)
				return typesList[i].typeId;
	}
	return -1;
}

ListObject * BTNodeTypes::GetListObjectPtr() const
{
	return listObject;
}

void BTNodeTypes::InsertType(const std::string& category, const std::string& name, int maxOutputs)
{
	NodeType newNodeType((int)typesList.size(), category, name, maxOutputs);
	typesList.push_back(newNodeType);

	if(category.length() > 0)
		listObject->AddItemInCategory(category, name);
}


