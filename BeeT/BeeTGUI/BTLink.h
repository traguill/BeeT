#ifndef __BTLINK_H__
#define __BTLINK_H__

#include "ThirdParty/ImGui/imgui.h"

class BTPin;

class BTLink
{
public:
	BTLink(int id, BTPin* sourcePin, BTPin* targetPin);
	~BTLink();

	void CleanUp(bool fromSourcePin);

public:
	int id = -1;
	BTPin* sourcePin = nullptr;
	BTPin* targetPin = nullptr;

	ImVec4 color = ImVec4(255.0f, 255.0f, 255.0f, 255.0f); // TODO: Change the color of the Link depending on the state
};
#endif

