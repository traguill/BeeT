#include "GameManager.h"
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include "Block.h"
#include "SDL/include/SDL.h"


#include <stdlib.h> // TEST RND
#include "Globals.h"
GameManager::GameManager(SDL_Renderer* renderer) : renderer(renderer)
{
}

GameManager::~GameManager()
{
	for (auto e : entities)
		delete e;
}

void GameManager::Init()
{
	player = new Player(renderer, 400, 300);
	AddEntity(player);

	// Enemies
	//enemy = new Enemy(renderer, 300, 100);
	//AddEntity(enemy);

	for (int i = 0; i < 20; i++)
	{
		Enemy* tst = new Enemy(renderer, rand() % SCREEN_WIDTH + 1, rand() % SCREEN_HEIGHT + 1);
		AddEntity(tst);
	}

	// Block
	block = new Block(renderer, 600, 400);
	AddEntity(block);
}

void GameManager::Tick(float dt)
{
	if (entitiesToAdd.empty() == false)
	{
		for (auto e : entitiesToAdd)
			entities.push_back(e);
		entitiesToAdd.clear();
	}
	if (entitiesToRemove.empty() == false)
	{
		for (auto e : entitiesToRemove)
		{
			if (e->type == ENEMY)
			{
				int a = 2;
			}
  			entities.erase(std::find(entities.begin(), entities.end(), e));
			delete e;
			e = NULL;
		}
		entitiesToRemove.clear();
	}
	for (auto entity : entities)
		entity->Tick(dt);
}

void GameManager::Draw()
{
	for (auto entity : entities)
		entity->Draw();
}

void GameManager::AddEntity(Entity * entity)
{
	entitiesToAdd.push_back(entity);
}

void GameManager::RemoveEntity(Entity * entity)
{
	auto found = std::find(entitiesToRemove.begin(), entitiesToRemove.end(), entity);
	if(found == entitiesToRemove.end())
		entitiesToRemove.push_back(entity);
}

void GameManager::OnInitBTTask(unsigned int btId, const char * taskId)
{
	auto task = taskOnInitFunctions.find(btId);
	if (task != taskOnInitFunctions.end())
		task->second(taskId);
}

NodeStatus GameManager::UpdateBTTask(unsigned int btId, const char * taskId)
{
	auto task = taskUpdateFunctions.find(btId);
	if (task != taskUpdateFunctions.end())
	{
		return task->second(taskId);
	}

	return NS_FAILURE;
}

void GameManager::OnFinishBTTask(unsigned int btId, const char * taskId)
{
	auto task = taskOnFinishFunctions.find(btId);
	if (task != taskOnFinishFunctions.end())
		task->second(taskId);
}
