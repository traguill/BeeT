#include "Block.h"
#include "Physics.h"

Block::Block(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = BLOCK;
	LoadSprite("Game/flower.bmp", 100, 100);
	g_Physics->AddBody(this, 50);

	CalculateCorners();
}

Block::~Block()
{
}

bool Block::IsBlockingSight(fPoint a, fPoint b) const
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
	float size = 50;
	cornerA = fPoint(pos.x - size, pos.y - size);
	cornerB = fPoint(pos.x + size, pos.y - size);
	cornerC = fPoint(pos.x - size, pos.y + size);
	cornerD = fPoint(pos.x + size, pos.y + size);
}
