#include "Enemy.h"
#include "Physics.h"
#include <functional>
#include "GameManager.h"
#include "Bullet.h"
#include "Block.h"
#include "Globals.h"

#include <stdlib.h>

Enemy::Enemy(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = ENEMY;
	LoadSprite("Game/Panda.bmp", 32, 52);
	g_Physics->AddBody(this, 16);

	btId = BEET_LoadBehaviorTreeFromFile("Enemy.json", BEET_FALSE);
	// OnInit
	std::function<void(const char*)> btTasksOnInitFunc = std::bind(&Enemy::BTTaskOnInit, this, std::placeholders::_1);
	g_GameManager->taskOnInitFunctions.insert(std::pair<int, std::function<void(const char*)>>(btId, btTasksOnInitFunc));
	// Update
	std::function<NodeStatus(const char*)> btTasksFunc = std::bind(&Enemy::BTTaskUpdate, this, std::placeholders::_1);
	g_GameManager->taskUpdateFunctions.insert(std::pair<int, std::function<NodeStatus(const char*)>>(btId, btTasksFunc));

	// OnFinish
	std::function<void(const char*)> btTasksOnFinishFunc = std::bind(&Enemy::BTTaskOnFinish, this, std::placeholders::_1);
	g_GameManager->taskOnFinishFunctions.insert(std::pair<int, std::function<void(const char*)>>(btId, btTasksOnFinishFunc));

	speed = 150.0f;
	dir.x = 0;
	dir.y = 0;
}

Enemy::~Enemy()
{
	BEET_CloseBehaviorTree(btId);
}

void Enemy::UpdateLogic(float dt)
{}

void Enemy::OnCollision(Entity * otherEntity)
{
}

void Enemy::BTTaskOnInit(const char * taskId)
{
	if (strcmp(taskId, "Move1") == 0)
	{
		float2 dst = BEET_BBGetVector2(btId, "Point1");
		SetDestination(fPoint(dst.x * 32.0f, dst.y * 32.0f));
	}
	else
	{
		float2 dst = BEET_BBGetVector2(btId, "Point2");
		SetDestination(fPoint(dst.x * 32.0f, dst.y * 32.0f));
	}
}

NodeStatus Enemy::BTTaskUpdate(const char * taskId)
{
	if (strcmp(taskId, "Move1") == 0)
	{
		return HasArrived() ? NS_SUCCESS : NS_RUNNING;
	}
	else
	{
		return HasArrived() ? NS_SUCCESS : NS_RUNNING;
	}

	return NS_FAILURE;
}

void Enemy::BTTaskOnFinish(const char * taskId)
{

}