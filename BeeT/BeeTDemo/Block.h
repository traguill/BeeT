#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "Entity.h"

class Block : public Entity
{
public:
	Block(SDL_Renderer* renderer, float posX, float posY);
	~Block();

	bool IsBlockingSight(fPoint a, fPoint b) const;

private:

	void CalculateCorners();

private:
	/*
		A    B

		C    D
	
	*/
	fPoint cornerA;
	fPoint cornerB;
	fPoint cornerC;
	fPoint cornerD;
};
#endif // !__BLOCK_H__

