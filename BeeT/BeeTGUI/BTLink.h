#ifndef __BTLINK_H__
#define __BTLINK_H__

#include "ThirdParty/ImGui/imgui.h"
#include "Data.h"
#include <map>

class BTPin;

class BTLink
{
public:
	BTLink(int id, BTPin* sourcePin, BTPin* targetPin);
	BTLink(Data& data, const std::map<int, BTPin*>& pinsList);
	~BTLink();

	void CleanUp();
	void Save(Data& file)const;
	
	bool IsFlowing();
	ImVec4 GetColor();

public:
	int id = -1;
	BTPin* sourcePin = nullptr;
	BTPin* targetPin = nullptr;

};
#endif

