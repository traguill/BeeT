#include "BTLink.h"
#include "BTNode.h"

BTLink::BTLink()
{
}

BTLink::BTLink(int id, int sourcePinId, int targetPinId) : id(id), sourcePinId(sourcePinId), targetPinId(targetPinId)
{}

BTLink::~BTLink()
{
}

BTPin::BTPin()
{}

BTPin::BTPin(int id, BTNode * node, ax::NodeEditor::PinKind kind) : id(id), node(node), kind(kind)
{
}
