#include "BTLink.h"
#include "BTPin.h"
#include "Log.h"

#include <vector>
#include <algorithm>

BTLink::BTLink(int id, BTPin* sourcePin, BTPin* targetPin) : id(id), sourcePin(sourcePin), targetPin(targetPin)
{}

BTLink::BTLink(Data & data, const std::map<int, BTPin*>& pinsList)
{
	id = data.GetInt("id");
	int sourceId = data.GetInt("sourceId");
	int targetId = data.GetInt("targetId");
	std::map<int, BTPin*>::const_iterator found = pinsList.find(sourceId);
	if (found != pinsList.end())
		sourcePin = found->second;
	found = pinsList.find(targetId);
	if (found != pinsList.end())
		targetPin = found->second;

	if (sourcePin && targetPin)
	{
		sourcePin->links.push_back(this);
		targetPin->links.push_back(this);
	}
	else
	{
		LOGW("Link (%i): pins are not loaded correctly", id);
	}
}

BTLink::~BTLink()
{
}

void BTLink::CleanUp(bool fromSourcePin)
{
	if (fromSourcePin)
	{
		if (targetPin)
		{
			std::vector<BTLink*>::iterator found = std::find(targetPin->links.begin(), targetPin->links.end(), this);
			if (found != targetPin->links.end())
			{
				targetPin->links.erase(found);
			}
			targetPin = nullptr;
		}
	}
	else
	{
		if (sourcePin)
		{
			std::vector<BTLink*>::iterator found = std::find(sourcePin->links.begin(), sourcePin->links.end(), this);
			if (found != sourcePin->links.end())
			{
				sourcePin->links.erase(found);
			}
			sourcePin = nullptr;
		}	
	}
}

void BTLink::Save(Data & file) const
{
	Data data;
	data.AppendInt("id", id);
	data.AppendInt("sourceId", sourcePin->id);
	data.AppendInt("targetId", targetPin->id);
	file.AppendArrayValue(data);
}
