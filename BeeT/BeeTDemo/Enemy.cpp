#include "Enemy.h"
#include "Physics.h"
#include <functional>
#include "GameManager.h"

Enemy::Enemy(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = ENEMY;
	LoadSprite("Game/flower.bmp", 100, 100);
	g_Physics->AddBody(this, 50);

	btId = BEET_LoadBehaviorTreeFromFile("Enemy.json", BEET_TRUE);
	std::function<NodeStatus(const char*)> btTasksFunc = std::bind(&Enemy::BTTask, this, std::placeholders::_1);
	g_GameManager->taskFunctions.insert(std::pair<int, std::function<NodeStatus(const char*)>>(btId, btTasksFunc));
}

Enemy::~Enemy()
{
}

void Enemy::OnCollision(Entity * otherEntity)
{
	if (otherEntity->type == PLAYER_BULLET)
	{
		int life = BEET_BBGetInt(btId, "life");
		life -= 60;
		BEET_BBSetInt(btId, "life", life);
		if(life <= 0)
			g_GameManager->RemoveEntity(this);
	}
}

NodeStatus Enemy::BTTask(const char * taskId)
{
	if (strcmp(taskId, "Rotate") == 0)
	{
		angle += 30;
		return NS_SUCCESS;
	}
	else if (strcmp(taskId, "Shoot") == 0)
	{
		dirX = 1;
		speed = 30;
		return NS_SUCCESS;
	}
	return NS_SUSPENDED;
}
