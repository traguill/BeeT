#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include <vector>
#include <map>
#include <functional>
#include "../BeeTLib/beet.h"

class Entity;
class Player;
class Enemy;
class Block;
struct SDL_Renderer;

class GameManager
{
public:
	GameManager(SDL_Renderer* renderer);
	~GameManager();

	void Init();

	void Tick(float dt);
	void Draw();

	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);

	void OnInitBTTask(unsigned int btId, const char* taskId, const BBVar* extraData);
	NodeStatus UpdateBTTask(unsigned int btId, const char* taskId, const BBVar* extraData);
	void OnFinishBTTask(unsigned int btId, const char* taskId, const BBVar* extraData);

	// Debug
	void DrawGridDebug(SDL_Renderer* renderer);

private:
	std::vector<Entity*> entities;
	std::vector<Entity*> entitiesToAdd;
	std::vector<Entity*> entitiesToRemove;

	SDL_Renderer* renderer;

public:
	std::map<int, std::function<void(const char*, const BBVar*)>> taskOnInitFunctions; // Update
	std::map<int, std::function<NodeStatus(const char*, const BBVar*)>> taskUpdateFunctions; // Update
	std::map<int, std::function<void(const char*, const BBVar*)>> taskOnFinishFunctions; // Update

	// Player
	Player* player;

	// Enemies
	Enemy* enemy;

	// Blocks
	Block* block;

};
extern GameManager* g_GameManager;
#endif // !__GAME_MANAGER_H__

