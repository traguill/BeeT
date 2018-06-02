#include "Enemy.h"
#include "Physics.h"

Enemy::Enemy(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = ENEMY;
	LoadSprite("Game/flower.bmp", 100, 100);
	g_Physics->AddBody(this, 50);
}

Enemy::~Enemy()
{
}

void Enemy::OnCollision(Entity * otherEntity)
{
	if (otherEntity->type == PLAYER_BULLET)
	{
		g_GameManager->RemoveEntity(this);
	}
}
