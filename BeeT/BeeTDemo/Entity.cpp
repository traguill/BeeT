#include "Entity.h"
#include "Physics.h"

Entity::Entity(SDL_Renderer* renderer, float posX, float posY) : renderer(renderer)
{
	pos.x = posX;
	pos.y = posY;
}

Entity::~Entity()
{
	if (texture)
		SDL_DestroyTexture(texture);
	if (hasCollider)
		g_Physics->RemoveBody(this);
}

bool Entity::LoadSprite(const char * path, int sizeX, int sizeY)
{
	bool ret = false;
	SDL_Surface* surf = SDL_LoadBMP(path);
	if (surf)
	{
		texture = SDL_CreateTextureFromSurface(renderer, surf);
		if (texture)
		{
			rect = { int(pos.x - sizeX * 0.5f), int(pos.y - sizeY * 0.5f), sizeX, sizeY };
			ret = true;
		}
	}
	return ret;
}

void Entity::Tick(float dt)
{
	UpdateLogic(dt);
	if(!isStop)
		Move(dt);
}

void Entity::Draw()
{
	if (texture)
		SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
}

void Entity::OnCollision(Entity * otherEntity)
{
}

void Entity::UpdateLogic(float dt)
{}

void Entity::Move(float dt)
{
	pos += dir * dt * speed;

	rect.x = pos.x - (rect.w * 0.5f);
	rect.y = pos.y - (rect.h * 0.5f);
}
