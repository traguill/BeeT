#include "Bullet.h"
#include "Physics.h"

Bullet::Bullet(SDL_Renderer* renderer, float posX, float posY, bool fromPlayer) : Entity(renderer, posX, posY)
{
	if (fromPlayer)
	{
		type = PLAYER_BULLET;
 		LoadSprite("Game/beeBullet.bmp", 32, 32);
	}
	else
	{
		type = ENEMY_BULLET;
		LoadSprite("Game/flowerBullet.bmp", 32, 32);
	}
	g_Physics->AddBody(this, 16);
	isStop = false;
}

Bullet::~Bullet()
{}

void Bullet::UpdateLogic(float dt)
{
	timer += dt;
	if (timer >= 3000.0f)
		g_GameManager->RemoveEntity(this);
}

void Bullet::OnCollision(Entity * otherEntity)
{
	if (otherEntity->type == ENEMY && type == PLAYER_BULLET)
	{
		g_GameManager->RemoveEntity(this);
	}
	else if (otherEntity->type == PLAYER && type == ENEMY_BULLET)
	{
		g_GameManager->RemoveEntity(this);
	}
}
