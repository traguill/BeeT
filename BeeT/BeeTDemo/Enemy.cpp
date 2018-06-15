#include "Enemy.h"
#include "Physics.h"
#include <functional>
#include "GameManager.h"
#include "Bullet.h"
#include "Block.h"
#include "Globals.h"

Enemy::Enemy(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = ENEMY;
	LoadSprite("Game/flower.bmp", 100, 100);
	g_Physics->AddBody(this, 50);

	btId = BEET_LoadBehaviorTreeFromFile("Enemy.json", BEET_TRUE);
	std::function<NodeStatus(const char*)> btTasksFunc = std::bind(&Enemy::BTTaskUpdate, this, std::placeholders::_1);
	g_GameManager->taskUpdateFunctions.insert(std::pair<int, std::function<NodeStatus(const char*)>>(btId, btTasksFunc));

	enemySpeed = 350.0f;
	dir.x = 0.3f;
	dir.y = 0.67f; // Set a rnd
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

void Enemy::BTTaskOnInit(const char * taskId)
{
	if (strcmp(taskId, "Hide") == 0)
	{
		FindCover();
	}
}

NodeStatus Enemy::BTTaskUpdate(const char * taskId)
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
		Bullet* bullet = new Bullet(renderer, pos.x + 60, pos.y, false);
		bullet->dir = dir;
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

void Enemy::BTTaskOnFinish(const char * taskId)
{
}

void Enemy::Movement()
{
	if (speed == 0)
	{
		speed = enemySpeed;
	}

	float halfSize = 50.0f;
	if (pos.x - halfSize < 0.0f)
	{
		pos.x = 0.0f + halfSize;
		dir.x *= -1;
	}
	if (pos.x + halfSize > SCREEN_WIDTH)
	{
		pos.x = SCREEN_WIDTH - halfSize;
		dir.x *= -1;
	}
	if (pos.y - halfSize < 0.0f)
	{
		pos.y = 0.0f + halfSize;
		dir.y *= -1;
	}
	if (pos.y + halfSize > SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - halfSize;
		dir.y *= -1;
	}
}

void Enemy::Burst()
{
	ShootBullet(pos.x + 60, pos.y, 1.0f, 0.0f, 600.0f); // Right
	ShootBullet(pos.x - 60, pos.y, -1.0f, 0.0f, 600.0f); // Left
	ShootBullet(pos.x, pos.y + 60, 0.0f, 1.0f, 600.0f); // Down
	ShootBullet(pos.x, pos.y - 60, 0.0f, -1.0f, 600.0f); // Up
}

void Enemy::ShootBullet(float posX, float posY, float dirX, float dirY, float speed)
{
	Bullet* bullet = new Bullet(renderer, posX, posY, false);
	bullet->dir.x = dirX;
	bullet->dir.y = dirY;
	bullet->speed = speed;
	g_GameManager->AddEntity(bullet);
}

void Enemy::FindCover()
{

}

bool Enemy::IsPlayerOnSight()
{
	Entity* player = (Entity*)g_GameManager->player;
	Block* block = g_GameManager->block;

	return block->IsBlockingSight(pos, player->pos);
}
