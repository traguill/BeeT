#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

class Player : public Entity
{
public:
	Player(SDL_Renderer* renderer, float posX, float posY);
	~Player();

	void UpdateLogic(float dt);

public:
	float rotSpeed;
};
#endif // !__PLAYER_H__
