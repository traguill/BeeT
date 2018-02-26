#ifndef __BTLINK_H__
#define __BTLINK_H__

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/NodeEditor/Include/NodeEditor.h"

class BTNode;

struct BTPin
{
	int id = -1;
	BTNode* node = nullptr;
	ax::NodeEditor::PinKind kind;

	BTPin();
	BTPin(int id, BTNode* node, ax::NodeEditor::PinKind kind);
};

class BTLink
{
public:
	BTLink();
	BTLink(int id, int sourcePinId, int targetPinId);
	~BTLink();

public:
	int id = -1;
	int sourcePinId = -1;
	int targetPinId = -1;

	ImVec4 color = ImVec4(255.0f, 255.0f, 255.0f, 255.0f); // TODO: Change the color of the Link depending on the state
};
#endif

