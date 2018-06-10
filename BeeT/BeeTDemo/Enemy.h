#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "../BeeTLib/beet.h"
#include "../SharedData/SharedEnums.h"

class Enemy : public Entity
{
public:
	Enemy(SDL_Renderer* renderer, float posX, float posY);
	~Enemy();

	void OnCollision(Entity* otherEntity);

	NodeStatus BTTask(const char* taskId);

private:
	unsigned int btId;

};
#endif // !__ENEMY_H__

