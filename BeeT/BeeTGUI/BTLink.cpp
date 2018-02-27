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
		sourcePin = nullptr;
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
		targetPin = nullptr;
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
