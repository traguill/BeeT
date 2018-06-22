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
	LoadSprite("Game/flower.bmp", 50, 50);
	g_Physics->AddBody(this, 25);

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
	dir.x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	dir.y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

Enemy::~Enemy()
{
	BEET_CloseBehaviorTree(btId);
}

void Enemy::UpdateLogic(float dt)
{
	bool hidden = IsPlayerOnSight();
	BEET_BBSetBool(btId, "hidden", (BEET_bool)hidden);
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
	} else if(strcmp(taskId, "Move") == 0)
	{
		isStop = false;
	}
	else if (strcmp(taskId, "Chase") == 0)
	{
		isStop = false;
	}
}

NodeStatus Enemy::BTTaskUpdate(const char * taskId)
{
	if (BEET_BBGetInt(btId, "life") <= 0)
		return NS_FAILURE;
	
	if (strcmp(taskId, "Rotate") == 0)
	{
		angle += 10;
		return NS_SUCCESS;
	}
	else if (strcmp(taskId, "Shoot") == 0)
	{
		int ammo = BEET_BBGetInt(btId, "ammo");
		if (ammo > 0)
		{
			Bullet* bullet = new Bullet(renderer, pos.x + (dir.x * 60), pos.y + (dir.y * 60), false);
			bullet->dir = dir;
			bullet->speed = 600.0f;
			g_GameManager->AddEntity(bullet);
			ammo--;
			BEET_BBSetInt(btId, "ammo", ammo);
			return NS_SUCCESS;
		}
		else
			return NS_FAILURE;
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
	else if (strcmp(taskId, "Hide") == 0)
	{
		if (FollowRoute())
			return NS_SUCCESS;
		else
			return NS_RUNNING;
	}
	else if (strcmp(taskId, "Reload") == 0)
	{
		BEET_BBSetInt(btId, "ammo", 5);
		return NS_SUCCESS;
	}
	else if (strcmp(taskId, "Chase") == 0)
	{
		Chase();
		return NS_RUNNING;
	}
	return NS_SUSPENDED;
}

void Enemy::BTTaskOnFinish(const char * taskId)
{
	if (strcmp(taskId, "Move") == 0)
	{
		isStop = true;
	}
	else if (strcmp(taskId, "Chase") == 0)
	{
		isStop = true;
	}
	else if (strcmp(taskId, "Hide") == 0)
	{
		isStop = true;
	}
}

void Enemy::Movement()
{

	float halfSize = 5.0f;
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
	int ammo = BEET_BBGetInt(btId, "ammo");
	if (ammo > 0)
	{
		ShootBullet(pos.x + 60, pos.y, 1.0f, 0.0f, 600.0f); // Right
		ShootBullet(pos.x - 60, pos.y, -1.0f, 0.0f, 600.0f); // Left
		ShootBullet(pos.x, pos.y + 60, 0.0f, 1.0f, 600.0f); // Down
		ShootBullet(pos.x, pos.y - 60, 0.0f, -1.0f, 600.0f); // Up
		ammo--;
		BEET_BBSetInt(btId, "ammo", ammo);
	}
}

void Enemy::Chase()
{
	fPoint player = ((Entity*)g_GameManager->player)->pos;
	dir = player - pos;
	dir.normalize();
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
	Block* block = g_GameManager->block;
	route.clear();
	hasDestination = false;
	route = block->GetHideRoute(pos, ((Entity*)g_GameManager->player)->pos);
}

bool Enemy::IsPlayerOnSight()
{
	Entity* player = (Entity*)g_GameManager->player;
	Block* block = g_GameManager->block;

	return block->IsBlockingSight(pos, player->pos);
}

bool Enemy::FollowRoute()
{
	if (!hasDestination)
	{
		if (route.size() == 0)
		{
			isStop = true;
			return true;
		}
		destination = route[0];
		route.erase(route.begin());
		dir = destination - pos;
		dir.normalize();
		isStop = false;
		hasDestination = true;
	}

	// Check destination
	if (pos.DistanceTo(destination) < 2.0f)
	{
		hasDestination = false;
		if (route.size() == 1)
		{
			isStop = true;
			return true;
		}
	}
	return false;
}
