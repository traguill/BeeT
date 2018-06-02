#include "Bullet.h"
#include "Physics.h"

Bullet::Bullet(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
 	LoadSprite("Game/beeBullet.bmp", 32, 32);
	type = PLAYER_BULLET;
	g_Physics->AddBody(this, 16);
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
	if (otherEntity->type == ENEMY)
	{
		g_GameManager->RemoveEntity(this);
	}
}
