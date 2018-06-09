#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include <vector>
#include <map>
#include <functional>
#include "../BeeTLib/beet.h"

class Entity;

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Tick(float dt);
	void Draw();

	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

	NodeStatus UpdateBTTask(unsigned int btId, const char* taskId);

private:
	std::vector<Entity*> entities;
	std::vector<Entity*> entitiesToAdd;
	std::vector<Entity*> entitiesToRemove;

public:
	// TODO: Make it private
	std::map<int, std::function<NodeStatus(const char*)>> taskFunctions;
};
extern GameManager* g_GameManager;
#endif // !__GAME_MANAGER_H__

