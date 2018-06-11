#include "BTPin.h"
#include "BTNode.h"

BTPin::BTPin()
{}

BTPin::BTPin(int id, ax::NodeEditor::PinKind kind, BTNode * node, bool fromParallel) : id(id), kind(kind), node(node), isSimpleParallel(fromParallel)
{}

BTPin::BTPin(BTNode* node, Data & data) : node(node)
{
	id = data.GetInt("id");
	kind = (ax::NodeEditor::PinKind)data.GetInt("kind");
}

BTPin::~BTPin()
{}

bool BTPin::IsLinkAvailable() const
{
	bool ret = false;
	switch (kind)
	{
	case ax::NodeEditor::PinKind::Target: //Input
		ret = (links.size() == 0);
		break;
	case ax::NodeEditor::PinKind::Source: //Output
		if (isSimpleParallel == false)
			ret = ((unsigned int)node->type->maxOutputs > links.size() || node->type->maxOutputs == -1);
		else
			ret = (1 > links.size());
		break;
	}
	return ret;
}

void BTPin::Save(Data & file) const
{
	Data data;
	data.AppendInt("id", id);
	data.AppendInt("kind", static_cast<int>(kind));

	file.AppendArrayValue(data);
}
