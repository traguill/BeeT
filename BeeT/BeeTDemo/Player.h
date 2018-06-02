#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

class Player : public Entity
{
public:
	Player(float posX, float posY);
	~Player();

	void UpdateLogic(float dt);

private:
	float rotSpeed;

};
#endif // !__PLAYER_H__
