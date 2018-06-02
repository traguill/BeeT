#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include <vector>

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

private:
	std::vector<Entity*> entities;
	std::vector<Entity*> entitiesToAdd;
	std::vector<Entity*> entitiesToRemove;
};
extern GameManager* g_GameManager;
#endif // !__GAME_MANAGER_H__

