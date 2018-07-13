#include "Enemy.h"
#include "Physics.h"
#include <functional>
#include "GameManager.h"
#include "Bullet.h"
#include "Block.h"
#include "Dijkstra.h"

#include <stdlib.h>

#include <iostream>

Enemy::Enemy(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = ENEMY;
	LoadSprite("Game/Panda.bmp", 32, 52);
	g_Physics->AddBody(this, 16);

	btId = BEET_LoadBehaviorTreeFromFile("Enemy.json", BEET_TRUE);
	// OnInit
	std::function<void(const char*)> btTasksOnInitFunc = std::bind(&Enemy::BTTaskOnInit, this, std::placeholders::_1);
	g_GameManager->taskOnInitFunctions.insert(std::pair<int, std::function<void(const char*)>>(btId, btTasksOnInitFunc));
	// Update
	std::function<NodeStatus(const char*)> btTasksFunc = std::bind(&Enemy::BTTaskUpdate, this, std::placeholders::_1);
	g_GameManager->taskUpdateFunctions.insert(std::pair<int, std::function<NodeStatus(const char*)>>(btId, btTasksFunc));

	// OnFinish
	std::function<void(const char*)> btTasksOnFinishFunc = std::bind(&Enemy::BTTaskOnFinish, this, std::placeholders::_1);
	g_GameManager->taskOnFinishFunctions.insert(std::pair<int, std::function<void(const char*)>>(btId, btTasksOnFinishFunc));

	speed = 50.0f;
	dir.x = 0;
	dir.y = 0;
}

Enemy::~Enemy()
{
	BEET_CloseBehaviorTree(btId);
}

void Enemy::UpdateLogic(float dt)
{
	IsPlayerVisible();
}

void Enemy::OnCollision(Entity * otherEntity)
{
}

void Enemy::BTTaskOnInit(const char * taskId)
{
	if (strcmp(taskId, "Move1") == 0)
	{
		float2 dst = BEET_BBGetVector2(btId, "Point1");
		routeIdx = 0;
		destination = iPoint(dst.x, dst.y);
		hasDestination = false;
		bool found = g_dijkstra->FindPath(GetTile(), destination, route);
		if (found == false)
			printf("Path not found");
	}
	else
	{
		float2 dst = BEET_BBGetVector2(btId, "Point2");
		routeIdx = 0;
		destination = iPoint(dst.x, dst.y);
		hasDestination = false;
		bool found = g_dijkstra->FindPath(GetTile(), destination, route);
		if (found == false)
			printf("Path not found");
	}
}

NodeStatus Enemy::BTTaskUpdate(const char * taskId)
{
	if (strcmp(taskId, "Move1") == 0)
	{
		if (!hasDestination)
		{
			SetDestination(fPoint(route[routeIdx].x * 32 + 16, route[routeIdx].y * 32 + 16));
			hasDestination = true;
			routeIdx++;
		}
		if (HasArrived())
		{
			hasDestination = false;
			if (routeIdx == route.size())
			{
				return NS_SUCCESS;
			}
		}
		return NS_RUNNING;
	}
	else
	{
		if (!hasDestination)
		{
			SetDestination(fPoint(route[routeIdx].x * 32 + 16, route[routeIdx].y * 32 + 16));
			hasDestination = true;
			routeIdx++;
		}
		if (HasArrived())
		{
			hasDestination = false;
			if (routeIdx == route.size())
			{
				return NS_SUCCESS;
			}
		}
		return NS_RUNNING;
	}

	return NS_FAILURE;
}

void Enemy::BTTaskOnFinish(const char * taskId)
{

}

bool Enemy::IsPlayerVisible() const
{
	fPoint playerPosition = ((Entity*)g_GameManager->player)->pos;
	fVec playerVec = playerPosition - pos;
	float angle = dir.AngleBetween(playerVec);

	if (fabs(angle) < 90.0f)
		printf("true\n");
	else
		printf("false\n");
	return true;
}
