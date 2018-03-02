#include "BTLink.h"
#include "BTPin.h"

#include <vector>
#include <algorithm>

BTLink::BTLink(int id, BTPin* sourcePin, BTPin* targetPin) : id(id), sourcePin(sourcePin), targetPin(targetPin)
{}

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
