#include "BTPin.h"
#include "BTNode.h"

BTPin::BTPin()
{}

BTPin::BTPin(int id, ax::NodeEditor::PinKind kind, BTNode * node) : id(id), kind(kind), node(node)
{}

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
		ret = (node->type->maxOutputs > links.size() || node->type->maxOutputs == -1);
		break;
	}
	return ret;
}