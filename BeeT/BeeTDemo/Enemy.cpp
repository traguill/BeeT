#include "Enemy.h"
#include "Physics.h"
#include "GameManager.h"
#include "Bullet.h"
#include "Block.h"
#include "Dijkstra.h"

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
	//IsPlayerVisible();

	if (ceil(dir.x) == 1.0f) currentAnim = &walkRight;
	if (ceil(dir.x) == -1.0f) currentAnim = &walkLeft;
	if (ceil(dir.y) == 1.0f) currentAnim = &walkDown;
	if (ceil(dir.y) == -1.0f) currentAnim = &walkUp;
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
	// Walk
	walkDown.frames.push_back({  0, 100, 16, 25 });
	walkDown.frames.push_back({ 16, 100, 16, 25 });
	walkDown.frames.push_back({ 32, 100, 16, 25 });
	walkDown.frames.push_back({  0, 125, 16, 25 });
	walkDown.frames.push_back({ 16, 125, 16, 25 });
	walkDown.frames.push_back({ 32, 125, 16, 25 });
	walkDown.speed = 10.0f;

	walkLeft.frames.push_back({  0, 150, 16, 27 });
	walkLeft.frames.push_back({ 16, 150, 16, 27 });
	walkLeft.frames.push_back({ 32, 150, 16, 27 });
	walkLeft.frames.push_back({  0, 177, 16, 27 });
	walkLeft.frames.push_back({ 16, 177, 16, 27 });
	walkLeft.frames.push_back({ 32, 177, 16, 27 });
	walkLeft.speed = 8.0f;

	walkRight.frames.push_back({ 48, 150, 16, 27 });
	walkRight.frames.push_back({ 64, 150, 16, 27 });
	walkRight.frames.push_back({ 80, 150, 16, 27 });
	walkRight.frames.push_back({ 48, 177, 16, 27 });
	walkRight.frames.push_back({ 64, 177, 16, 27 });
	walkRight.frames.push_back({ 80, 177, 16, 27 });
	walkRight.speed = 8.0f;

	walkUp.frames.push_back({ 48, 100, 16, 25 });
	walkUp.frames.push_back({ 64, 100, 16, 25 });
	walkUp.frames.push_back({ 80, 100, 16, 25 });
	walkUp.frames.push_back({ 48, 125, 16, 25 });
	walkUp.frames.push_back({ 64, 125, 16, 25 });
	walkUp.frames.push_back({ 80, 125, 16, 25 });
	walkUp.speed = 10.0f;
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
