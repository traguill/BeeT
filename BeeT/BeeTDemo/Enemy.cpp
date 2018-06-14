#include "Enemy.h"
#include "Physics.h"
#include <functional>
#include "GameManager.h"
#include "Bullet.h"
#include "Globals.h"

Enemy::Enemy(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = ENEMY;
	LoadSprite("Game/flower.bmp", 100, 100);
	g_Physics->AddBody(this, 50);

	btId = BEET_LoadBehaviorTreeFromFile("Enemy.json", BEET_TRUE);
	std::function<NodeStatus(const char*)> btTasksFunc = std::bind(&Enemy::BTTask, this, std::placeholders::_1);
	g_GameManager->taskFunctions.insert(std::pair<int, std::function<NodeStatus(const char*)>>(btId, btTasksFunc));

	enemySpeed = 350.0f;
	dirX = 0.3f;
	dirY = 0.67f; // Set a rnd
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
		bullet->dirX = dirX;
		bullet->dirY = dirY;
		bullet->speed = 600.0f;
		g_GameManager->AddEntity(bullet);
		return NS_SUCCESS;
	}
	else if (strcmp(taskId, "Move") == 0)
	{
		Movement();
		return NS_RUNNING;
	}
	else if (strcmp(taskId, "Burst") == 0)
	{
		Burst();
		return NS_SUCCESS;
	}
	return NS_SUSPENDED;
}

void Enemy::Movement()
{
	if (speed == 0)
	{
		speed = enemySpeed;
	}

	float halfSize = 50.0f;
	if (posX - halfSize < 0.0f)
	{
		posX = 0.0f + halfSize;
		dirX *= -1;
	}
	if (posX + halfSize > SCREEN_WIDTH)
	{
		posX = SCREEN_WIDTH - halfSize;
		dirX *= -1;
	}
	if (posY - halfSize < 0.0f)
	{
		posY = 0.0f + halfSize;
		dirY *= -1;
	}
	if (posY + halfSize > SCREEN_HEIGHT)
	{
		posY = SCREEN_HEIGHT - halfSize;
		dirY *= -1;
	}
}

void Enemy::Burst()
{
	ShootBullet(posX + 60, posY, 1.0f, 0.0f, 600.0f); // Right
	ShootBullet(posX - 60, posY, -1.0f, 0.0f, 600.0f); // Left
	ShootBullet(posX, posY + 60, 0.0f, 1.0f, 600.0f); // Down
	ShootBullet(posX, posY - 60, 0.0f, -1.0f, 600.0f); // Up
}

void Enemy::ShootBullet(float posX, float posY, float dirX, float dirY, float speed)
{
	Bullet* bullet = new Bullet(renderer, posX, posY, false);
	bullet->dirX = dirX;
	bullet->dirY = dirY;
	bullet->speed = speed;
	g_GameManager->AddEntity(bullet);
}
