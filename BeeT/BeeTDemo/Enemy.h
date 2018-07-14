#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "../BeeTLib/beet.h"
#include "../SharedData/SharedEnums.h"
#include "Animation.h"

#include <functional>
#include <string>
#include <map>
#include <vector>

class Enemy : public Entity
{
public:
	Enemy(SDL_Renderer* renderer, float posX, float posY);
	~Enemy();

	void UpdateLogic(float dt);

	void OnCollision(Entity* otherEntity);

	void BTTaskOnInit(const char* taskId, const BBVar* extraData);
	NodeStatus BTTaskUpdate(const char* taskId, const BBVar* extraData);
	void BTTaskOnFinish(const char* taskId, const BBVar* extraData);

private:
	void InitAnimations();
	void BindTaskFunctions();

	bool IsPlayerVisible()const;

	void InitMoveTo(const BBVar* extraData);
	NodeStatus UpdateMoveTo(const BBVar* extraData);

	void InitChase(const BBVar* extraData);
	NodeStatus UpdateChase(const BBVar* extraData);
	void FinishChase(const BBVar* extraData);

	std::map<std::string, std::function<void(const BBVar*)>> OnInitFunctions;
	std::map<std::string, std::function<NodeStatus(const BBVar*)>> OnUpdateFunctions;
	std::map<std::string, std::function<void(const BBVar*)>> OnFinishFunctions;

private:
	unsigned int btId;
	std::vector<iPoint> route;
	int routeIdx = 0;
	iPoint destination;
	bool hasDestination = false;

	Animation idleDown;
	Animation idleUp;
	Animation idleRight;
	Animation idleLeft;

	Animation walkDown;
	Animation walkUp;
	Animation walkRight;
	Animation walkLeft;

	Animation* currentAnim;

	bool playerVisible = false;
	iPoint lastSeenPlayerTile; 
	fPoint lastDir;
	float timer = 0.0f;
};
#endif // !__ENEMY_H__

