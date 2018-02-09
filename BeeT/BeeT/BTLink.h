#ifndef __BTLINK_H__
#define __BTLINK_H__

#include "ThirdParty/ImGui/imgui.h"

class BTNode;

class BTLink
{
public:
	BTLink();
	~BTLink();

public:
	int id = -1;
	BTNode* inputNode = nullptr;
	BTNode* outputNode = nullptr;

	ImVec4 color = ImVec4(255.0f, 255.0f, 255.0f, 255.0f); // TODO: Change the color of the Link depending on the state
};
#endif

