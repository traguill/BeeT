#include "Input.h"
#include "Player.h"
#include "Bullet.h"
#include "Physics.h"

Player::Player(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = PLAYER;
	LoadSprite("Game/Monkey.bmp", 64, 64);

	g_Physics->AddBody(this, 16);
	speed = 300.0f;
}

Player::~Player()
{
}

void Player::UpdateLogic(float dt)
{
	fPoint newDir = fPoint(0.0f, 0.0f);
	if (g_Input->GetKey(SDL_SCANCODE_D))
		newDir.x = 1.0f;
	else
	if (g_Input->GetKey(SDL_SCANCODE_A))
		newDir.x = -1.0f;
	else
	if (g_Input->GetKey(SDL_SCANCODE_S))
		newDir.y = 1.0f;
	else
	if (g_Input->GetKey(SDL_SCANCODE_W))
		newDir.y = -1.0f;
	dir = newDir;
	isStop = (newDir.x == 0.0f && newDir.y == 0.0f) ? true : false;	
}
