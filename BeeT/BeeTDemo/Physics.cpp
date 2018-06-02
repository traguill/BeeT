#include "Physics.h"
#include "Entity.h"

#include "SDL/include/SDL.h"

Physics::Physics()
{
}

Physics::~Physics()
{
	for (auto body : bodies)
		delete body;
}

void Physics::Tick()
{
	if (bodiesToAdd.empty() == false)
	{
		for (auto b : bodiesToAdd)
			bodies.push_back(b);
		bodiesToAdd.clear();
	}

	if (bodiesToRemove.empty() == false)
	{
		for (auto b : bodiesToRemove)
			bodies.erase(std::find(bodies.begin(), bodies.end(), b));
		bodiesToRemove.clear();
	}

	// Check collisions
	for (auto bodyA : bodies)
	{
		for (auto bodyB : bodies)
		{
			if (bodyA == bodyB)
				continue;
			float vX = bodyB->entity->posX - bodyA->entity->posX;
			float vY = bodyB->entity->posY - bodyA->entity->posY;
			float dst = sqrtf(vX * vX + vY * vY);
			if (dst <= bodyA->radius + bodyB->radius)
				bodyA->entity->OnCollision(bodyB->entity);
		}
	}
}

void Physics::AddBody(Entity * entity, int radius)
{
	// BE CAREFUL! I do not check for duplicates. Each entity should only have one Collision Body
	PhysBody* b = new PhysBody();
	b->entity = entity;
	b->radius = radius;
	entity->hasCollider = true;
	bodiesToAdd.push_back(b);
}

void Physics::RemoveBody(Entity * entity)
{
	PhysBody* toRemove = NULL;
	for (auto b : bodies)
	{
		if (b->entity == entity)
		{
			toRemove = b;
			break;
		}
	}
	if (toRemove)
		bodiesToRemove.push_back(toRemove);
}

void Physics::DrawColliders()
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Point points[720];
	float factor = (float)M_PI / 180.0f;
	for (auto b : bodies)
	{
		for (int i = 0; i < 720; i++)
		{
			points[i].x = b->entity->posX + b->radius * cos(i * factor);
			points[i].y = b->entity->posY + b->radius * sin(i * factor);
		}
		SDL_RenderDrawPoints(renderer, points, 720);
	}
}
