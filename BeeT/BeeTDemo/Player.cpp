#include "Input.h"
#include "Player.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

Player::Player(float posX, float posY) : Entity(posX, posY)
{
	rotSpeed = 0.1f;
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

	dirX = (float)cos((angle - 90)* DEGTORAD);
	dirY = (float)sin((angle - 90) * DEGTORAD);

	if (g_Input->GetKey(SDL_SCANCODE_W))
		speed = 0.1f;
	else
		speed = 0.0f;
	
}
