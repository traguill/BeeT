#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(SDL_Renderer* renderer, float posX, float posY);
	~Enemy();

	void OnCollision(Entity* otherEntity);

private:

};
#endif // !__ENEMY_H__

