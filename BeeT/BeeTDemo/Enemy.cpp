#include "Enemy.h"
#include "Physics.h"
#include <functional>
#include "GameManager.h"
#include "Bullet.h"

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
		life -= 10;
		BEET_BBSetInt(btId, "life", life);
		if(life <= 0)
			g_GameManager->RemoveEntity(this);
	}
}

NodeStatus Enemy::BTTask(const char * taskId)
{
	if (BEET_BBGetInt(btId, "life") <= 0)
		return NS_FAILURE;
	speed = 0; // Hack
	if (strcmp(taskId, "Rotate") == 0)
	{
		angle += 15;
		return NS_SUCCESS;
	}
	else if (strcmp(taskId, "Shoot") == 0)
	{
		Bullet* bullet = new Bullet(renderer, posX + 60, posY, false);
		bullet->dirX = 1.0f;
		bullet->dirY = 0.0f;
		bullet->speed = 200.0f;
		g_GameManager->AddEntity(bullet);
		return NS_SUCCESS;
	}
	else if (strcmp(taskId, "Move") == 0)
	{
		dirX = 1;
		speed = 30;
		return NS_SUCCESS;
	}
	return NS_SUSPENDED;
}
