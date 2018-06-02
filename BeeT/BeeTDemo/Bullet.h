#ifndef __BULLET_H__
#define __BULLET_H__

#include "Entity.h"

class Bullet : public Entity
{
public:
	Bullet(SDL_Renderer* renderer, float posX, float posY);
	~Bullet();

	void UpdateLogic(float dt);

	void OnCollision(Entity* otherEntity);

private:
	float timer = 0.0f;
};

#endif // !__BULLET_H__

