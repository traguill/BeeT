#include "Entity.h"
#include "Physics.h"

Entity::Entity(SDL_Renderer* renderer, float posX, float posY) : renderer(renderer), posX(posX), posY(posY)
{}

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
			rect = { int(posX - sizeX * 0.5f), int(posY - sizeY * 0.5f), sizeX, sizeY };
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
	posX += speed * dt * dirX;
	posY += speed * dt * dirY;

	rect.x = posX - (rect.w * 0.5f);
	rect.y = posY - (rect.h * 0.5f);
}
