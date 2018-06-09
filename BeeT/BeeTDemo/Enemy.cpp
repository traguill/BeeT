#include "Enemy.h"
#include "Physics.h"


Enemy::Enemy(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = ENEMY;
	LoadSprite("Game/flower.bmp", 100, 100);
	g_Physics->AddBody(this, 50);

	unsigned int btId = BEET_LoadBehaviorTreeFromFile("Enemy.json", BEET_TRUE);
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

NodeStatus Enemy::Shoot(unsigned int btId, const char * taskId)
{
	return NS_RUNNING;
}
