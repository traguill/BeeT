#include "Enemy.h"
#include "Physics.h"
#include "GameManager.h"
#include "Bullet.h"
#include "Block.h"
#include "Dijkstra.h"
#include "Player.h"

#include <stdlib.h>

Enemy::Enemy(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = ENEMY;
	LoadSprite("Game/pandaSprite.bmp", 16, 25);
	g_Physics->AddBody(this, 16);

	btId = BEET_LoadBehaviorTreeFromFile("Enemy.json", BEET_TRUE);
	// Bind Game Manager Functions
	// OnInit
	std::function<void(const char*, const BBVar*)> btTasksOnInitFunc = std::bind(&Enemy::BTTaskOnInit, this, std::placeholders::_1, std::placeholders::_2);
	g_GameManager->taskOnInitFunctions.insert(std::pair<int, std::function<void(const char*, const BBVar*)>>(btId, btTasksOnInitFunc));
	// Update
	std::function<NodeStatus(const char*, const BBVar*)> btTasksFunc = std::bind(&Enemy::BTTaskUpdate, this, std::placeholders::_1, std::placeholders::_2);
	g_GameManager->taskUpdateFunctions.insert(std::pair<int, std::function<NodeStatus(const char*, const BBVar*)>>(btId, btTasksFunc));
	// OnFinish
	std::function<void(const char*, const BBVar*)> btTasksOnFinishFunc = std::bind(&Enemy::BTTaskOnFinish, this, std::placeholders::_1, std::placeholders::_2);
	g_GameManager->taskOnFinishFunctions.insert(std::pair<int, std::function<void(const char*, const BBVar*)>>(btId, btTasksOnFinishFunc));

	BindTaskFunctions();
	
	InitAnimations();
	
	scale = 2.0f;
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
	timer += dt;
	if (timer > 0.2f)
	{
		playerVisible = IsPlayerVisible();
		if (playerVisible)
			lastSeenPlayerTile = g_GameManager->player->GetTile();
		BEET_BBSetBool(btId, "isPlayerVisible", (BEET_bool)playerVisible);
		timer = 0.0f;
	}
	if (!isStop)
	{
		if (ceil(dir.x) == 1.0f) currentAnim = &walkRight;
		if (ceil(dir.x) == -1.0f) currentAnim = &walkLeft;
		if (ceil(dir.y) == 1.0f) currentAnim = &walkDown;
		if (ceil(dir.y) == -1.0f) currentAnim = &walkUp;
		lastDir = dir;
	}
	else
	{
		if (lastDir.x == 1.0f) currentAnim = &idleRight;
		if (lastDir.x == -1.0f) currentAnim = &idleLeft;
		if (lastDir.y == 1.0f) currentAnim = &idleDown;
		if (lastDir.y == -1.0f) currentAnim = &idleUp;
	}
	
	rect = currentAnim->GetCurrentFrame(dt);
}

void Enemy::OnCollision(Entity * otherEntity)
{
}

void Enemy::BTTaskOnInit(const char * taskId, const BBVar* extraData)
{
	auto task = OnInitFunctions.find(taskId);
	if (task != OnInitFunctions.end())
		task->second(extraData);
}

NodeStatus Enemy::BTTaskUpdate(const char * taskId, const BBVar* extraData)
{
	auto task = OnUpdateFunctions.find(taskId);
	if (task != OnUpdateFunctions.end())
		return task->second(extraData);
	else
		return NS_FAILURE;
}

void Enemy::BTTaskOnFinish(const char * taskId, const BBVar* extraData)
{
	auto task = OnFinishFunctions.find(taskId);
	if (task != OnFinishFunctions.end())
		task->second(extraData);
}

void Enemy::InitAnimations()
{
	// Idle
	idleDown.frames.push_back({ 0, 0, 16, 24 });
	idleDown.frames.push_back({ 16, 0, 16, 24 });
	idleDown.frames.push_back({ 0, 24, 16, 24 });
	idleDown.frames.push_back({ 16, 24, 16, 24 });
	idleDown.speed = 10.0f;

	idleLeft.frames.push_back({ 0, 48, 16, 26 });
	idleLeft.frames.push_back({ 16, 48, 16, 26 });
	idleLeft.frames.push_back({ 0, 74, 16, 26 });
	idleLeft.frames.push_back({ 16, 74, 16, 26 });
	idleLeft.speed = 8.0f;

	idleRight.frames.push_back({ 32, 48, 16, 26 });
	idleRight.frames.push_back({ 32, 48, 16, 26 });
	idleRight.frames.push_back({ 48, 74, 16, 26 });
	idleRight.frames.push_back({ 48, 74, 16, 26 });
	idleRight.speed = 8.0f;

	idleUp.frames.push_back({ 32,  0, 16, 24 });
	idleUp.frames.push_back({ 32,  0, 16, 24 });
	idleUp.frames.push_back({ 48, 24, 16, 24 });
	idleUp.frames.push_back({ 48, 24, 16, 24 });
	idleUp.speed = 10.0f;

	// Walk
	walkDown.frames.push_back({  0, 100, 16, 25 });
	walkDown.frames.push_back({ 16, 100, 16, 25 });
	walkDown.frames.push_back({ 32, 100, 16, 25 });
	walkDown.frames.push_back({  0, 125, 16, 25 });
	walkDown.frames.push_back({ 16, 125, 16, 25 });
	walkDown.frames.push_back({ 32, 125, 16, 25 });
	walkDown.speed = 12.0f;

	walkLeft.frames.push_back({  0, 150, 16, 27 });
	walkLeft.frames.push_back({ 16, 150, 16, 27 });
	walkLeft.frames.push_back({ 32, 150, 16, 27 });
	walkLeft.frames.push_back({  0, 177, 16, 27 });
	walkLeft.frames.push_back({ 16, 177, 16, 27 });
	walkLeft.frames.push_back({ 32, 177, 16, 27 });
	walkLeft.speed = 12.0f;

	walkRight.frames.push_back({ 48, 150, 16, 27 });
	walkRight.frames.push_back({ 64, 150, 16, 27 });
	walkRight.frames.push_back({ 80, 150, 16, 27 });
	walkRight.frames.push_back({ 48, 177, 16, 27 });
	walkRight.frames.push_back({ 64, 177, 16, 27 });
	walkRight.frames.push_back({ 80, 177, 16, 27 });
	walkRight.speed = 12.0f;

	walkUp.frames.push_back({ 48, 100, 16, 25 });
	walkUp.frames.push_back({ 64, 100, 16, 25 });
	walkUp.frames.push_back({ 80, 100, 16, 25 });
	walkUp.frames.push_back({ 48, 125, 16, 25 });
	walkUp.frames.push_back({ 64, 125, 16, 25 });
	walkUp.frames.push_back({ 80, 125, 16, 25 });
	walkUp.speed = 12.0f;
}

void Enemy::BindTaskFunctions()
{
	std::function<void(const BBVar*)> taskFunc;
	std::function<NodeStatus(const BBVar*)> updateFunc;

	// MoveTo
	taskFunc = std::bind(&Enemy::InitMoveTo, this, std::placeholders::_1);
	OnInitFunctions.insert(std::pair<std::string, std::function<void(const BBVar*)>>("MoveTo", taskFunc));
	updateFunc = std::bind(&Enemy::UpdateMoveTo, this, std::placeholders::_1);
	OnUpdateFunctions.insert(std::pair<std::string, std::function<NodeStatus(const BBVar*)>>("MoveTo", updateFunc));

	// Chase
	taskFunc = std::bind(&Enemy::InitChase, this, std::placeholders::_1);
	OnInitFunctions.insert(std::pair<std::string, std::function<void(const BBVar*)>>("Chase", taskFunc));
	updateFunc = std::bind(&Enemy::UpdateChase, this, std::placeholders::_1);
	OnUpdateFunctions.insert(std::pair<std::string, std::function<NodeStatus(const BBVar*)>>("Chase", updateFunc));
	taskFunc = std::bind(&Enemy::FinishChase, this, std::placeholders::_1);
	OnFinishFunctions.insert(std::pair<std::string, std::function<void(const BBVar*)>>("Chase", taskFunc));
}

bool Enemy::IsPlayerVisible() const
{
	if (g_GameManager->player->isHidded)
		return false;
	fPoint playerPosition = ((Entity*)g_GameManager->player)->pos;
	fVec playerVec = playerPosition - pos;
	float angle = dir.AngleBetween(playerVec);

	if (fabs(angle) < 180.0f)
	{
		// Trace line
		iPoint tile = GetTile();
		iPoint playerTile = g_GameManager->player->GetTile();
		vector<iPoint> tiles;
		TraceLine(tile.x, tile.y, playerTile.x, playerTile.y, tiles);
		for (auto t : tiles)
		{
			if (!g_dijkstra->IsTileWalkable(t.x, t.y))
				return false;
		}
		return true;
	}
	else
		return false;
}

void Enemy::InitMoveTo(const BBVar * extraData)
{
	float2 dst = *(float2*)extraData->data;
	routeIdx = 0;
	destination = iPoint(dst.x, dst.y);
	hasDestination = false;
	bool found = g_dijkstra->FindPath(GetTile(), destination, route);
	if (found == false)
		printf("Path not found");
}

NodeStatus Enemy::UpdateMoveTo(const BBVar * extraData)
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

void Enemy::InitChase(const BBVar * extraData)
{
	speed = 75.0f;
	walkDown.speed = 14.0f;
	walkRight.speed = 14.0f;
	walkLeft.speed = 14.0f;
	walkDown.speed = 14.0f;
}

NodeStatus Enemy::UpdateChase(const BBVar * extraData)
{
	if (!hasDestination)
	{
		bool found = false;
		if (playerVisible)
		{
			found = g_dijkstra->FindPath(GetTile(), g_GameManager->player->GetTile(), route);
		}
		else
		{
			found = g_dijkstra->FindPath(GetTile(), lastSeenPlayerTile, route);
		}
		if (!found || route.size() == 0)
			return NS_FAILURE;
		routeIdx = 0;
		SetDestination(fPoint(route[0].x * 32 + 16, route[0].y * 32 + 16));
		hasDestination = true;
		routeIdx++;
	}
	if (HasArrived())
	{
		hasDestination = false;
		if (routeIdx == route.size())
		{
			return NS_FAILURE;
		}
	}
	return NS_RUNNING;
}

void Enemy::FinishChase(const BBVar * extraData)
{
	speed = 50.0f;
	walkDown.speed = 12.0f;
	walkRight.speed = 12.0f;
	walkLeft.speed = 12.0f;
	walkDown.speed = 12.0f;
}
