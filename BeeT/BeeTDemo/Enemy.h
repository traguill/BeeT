#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "../BeeTLib/beet.h"
#include "../SharedData/SharedEnums.h"
#include <vector>

class Enemy : public Entity
{
public:
	Enemy(SDL_Renderer* renderer, float posX, float posY);
	~Enemy();

	void UpdateLogic(float dt);

	void OnCollision(Entity* otherEntity);

	void BTTaskOnInit(const char* taskId);
	NodeStatus BTTaskUpdate(const char* taskId);
	void BTTaskOnFinish(const char* taskId);

private:


private:
	unsigned int btId;
	std::vector<iPoint> route;
	int routeIdx = 0;
	iPoint destination;
	bool hasDestination = false;
};
#endif // !__ENEMY_H__

