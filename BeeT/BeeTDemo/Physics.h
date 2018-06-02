#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include <vector>

class Entity;
struct SDL_Renderer;

struct PhysBody			// All collisions are circles
{
	Entity* entity;
	int radius;
};

class Physics
{
public:
	Physics();
	~Physics();

	void Tick();

	void AddBody(Entity* entity, int radius);
	void RemoveBody(Entity* entity);

	void DrawColliders();

public:
	SDL_Renderer* renderer = NULL;

private:
	std::vector<PhysBody*> bodies;
	std::vector<PhysBody*> bodiesToAdd;
	std::vector<PhysBody*> bodiesToRemove;
};
extern Physics* g_Physics;
#endif // !__PHYSICS_H__

