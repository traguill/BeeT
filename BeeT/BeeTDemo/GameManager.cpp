#include "GameManager.h"
#include "Entity.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
	for (auto e : entities)
		delete e;
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
			entities.erase(std::find(entities.begin(), entities.end(), e));
			delete e;
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
	entitiesToRemove.push_back(entity);
}

NodeStatus GameManager::UpdateBTTask(unsigned int btId, const char * taskId)
{
	if (entities.size() > 0)
	{
		entities[1]->angle += 30;
		return NS_SUCCESS;
	}
		return NS_RUNNING;
}
