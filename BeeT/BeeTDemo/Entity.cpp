#include "Entity.h"

Entity::Entity(float posX, float posY) : posX(posX), posY(posY)
{}

Entity::~Entity()
{
	if (texture)
		SDL_DestroyTexture(texture);
}

bool Entity::LoadSprite(SDL_Renderer * renderer, const char * path, int sizeX, int sizeY)
{
	bool ret = false;
	SDL_Surface* surf = SDL_LoadBMP(path);
	if (surf)
	{
		texture = SDL_CreateTextureFromSurface(renderer, surf);
		if (texture)
		{
			rect = { (int)posX, (int)posY, sizeX, sizeY };
			ret = true;
		}
	}
	return ret;
}

void Entity::Tick(float dt)
{
	UpdateLogic(dt);
	Move(dt);
}

void Entity::Draw(SDL_Renderer* renderer)
{
	if (texture)
		SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
}

void Entity::UpdateLogic(float dt)
{}

void Entity::Move(float dt)
{
	posX += speed * dt * dirX;
	posY += speed * dt * dirY;

	rect.x = posX;
	rect.y = posY;
}
