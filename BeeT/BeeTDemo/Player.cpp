#include "Input.h"
#include "Player.h"
#include "Bullet.h"
#include "Physics.h"

Player::Player(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = PLAYER;
	rotSpeed = 50.0f;
	LoadSprite("Game/bee.bmp", 51, 74);

	g_Physics->AddBody(this, 25);
}

Player::~Player()
{
}

void Player::UpdateLogic(float dt)
{
	if (g_Input->GetKey(SDL_SCANCODE_D))
	{
		angle += rotSpeed * dt;
	}

	if (g_Input->GetKey(SDL_SCANCODE_A))
	{
		angle -= rotSpeed * dt;
	}

	dir.x = (float)cos((angle - 90)* DEGTORAD);
	dir.y = (float)sin((angle - 90) * DEGTORAD);

	if (g_Input->GetKey(SDL_SCANCODE_W))
		speed = 100.0f;
	else
		speed = 0.0f;
	
	if (g_Input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		Bullet* bullet = new Bullet(renderer, pos.x, pos.y, true);
		bullet->dir = dir;
		bullet->speed = 500.0f;
		g_GameManager->AddEntity(bullet);
	}
	
}
