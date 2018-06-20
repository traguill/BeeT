#include "Block.h"
#include "Physics.h"

Block::Block(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = BLOCK;
	LoadSprite("Game/block.bmp", 20, 20);
	g_Physics->AddBody(this, 10);

	CalculateCorners();
}

Block::~Block()
{
}

std::vector<fPoint> Block::GetHideRoute(const fPoint & origin, const fPoint & target) const
{
	// Pick the farthest from the target
	float maxDst = 0.0f;
	int idxFarthest = 0;
	// Pick the closest from the origin
	float minDst = 99999999999.9f;
	int idxClosest = 0;
	for (int i = 0; i < hideSpots.size(); i++)
	{
		float dst = target.DistanceTo(hideSpots[i]);
		if (dst > maxDst)
		{
			maxDst = dst;
			idxFarthest = i;
		}
		float dst2 = origin.DistanceTo(hideSpots[i]);
		if (dst2 < minDst)
		{
			minDst = dst2;
			idxClosest = i;
		}
	}
	// Trace route from two points
	std::vector<fPoint> route;
	int id = idxClosest - 1;
	do
	{
		id++;
		if (id == hideSpots.size())
			id = 0;
		route.push_back(hideSpots[id]);
	} while (id != idxFarthest);
	
	return route;
}

bool Block::IsBlockingSight(const fPoint& a, const fPoint& b) const
{
	int retA = PointInLine(cornerA, a, b);
	int retB = PointInLine(cornerB, a, b);
	int retC = PointInLine(cornerC, a, b);
	int retD = PointInLine(cornerD, a, b);

	if (retA >= 0 && retB >= 0 && retC >= 0 && retD >= 0) // Above
		return false;
	if (retA <= 0 && retB <= 0 && retC <= 0 && retD <= 0) // Below
		return false;

	return true;
}

void Block::CalculateCorners()
{
	/*	  0   1    2
			A    B
		  7        3
			C    D
		  6   5    4
	
	*/
	float size = 10;
	cornerA = fPoint(pos.x - size, pos.y - size);
	cornerB = fPoint(pos.x + size, pos.y - size);
	cornerC = fPoint(pos.x - size, pos.y + size);
	cornerD = fPoint(pos.x + size, pos.y + size);

	float dst = 2;
	hideSpots.resize(8);
	hideSpots[0] = fPoint(cornerA.x - dst, cornerA.y - dst);
	hideSpots[1] = fPoint(pos.x, cornerA.y - dst);
	hideSpots[2] = fPoint(cornerB.x + dst, cornerA.y - dst);
	hideSpots[3] = fPoint(cornerB.x + dst, pos.y);
	hideSpots[4] = fPoint(cornerB.x + dst, cornerD.y + dst);
	hideSpots[5] = fPoint(pos.x, cornerD.y + dst);
	hideSpots[6] = fPoint(cornerA.x - dst, cornerD.y + dst);
	hideSpots[7] = fPoint(cornerA.x - dst, pos.y);
}
