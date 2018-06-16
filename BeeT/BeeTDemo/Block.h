#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "Entity.h"
#include <vector>

class Block : public Entity
{
public:
	Block(SDL_Renderer* renderer, float posX, float posY);
	~Block();

	bool IsBlockingSight(const fPoint& a, const fPoint& b) const;
	std::vector<fPoint> GetHideRoute(const fPoint& origin, const fPoint& target) const;

private:

	void CalculateCorners();

private:
	/*	  0   1    2
			A    B
		  7        3
			C    D
		  6   5    4
	
	*/
	fPoint cornerA;
	fPoint cornerB;
	fPoint cornerC;
	fPoint cornerD;

	std::vector<fPoint> hideSpots;
};
#endif // !__BLOCK_H__

