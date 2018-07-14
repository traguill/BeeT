#include "Input.h"
#include "Player.h"
#include "Bullet.h"
#include "Physics.h"

#define BOX_ANIM_SPEED 10.0f

Player::Player(SDL_Renderer* renderer, float posX, float posY) : Entity(renderer, posX, posY)
{
	type = PLAYER;
	LoadSprite("Game/monkeySprite.bmp", 16, 27);

	g_Physics->AddBody(this, 16);
	speed = 100.0f;
	scale = 2.0f;
	InitAnimations();
	currentAnim = &idleLeft;
}

Player::~Player()
{
}

void Player::UpdateLogic(float dt)
{
	if (g_Input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		dir.x = dir.y = 0.0f;
		isStop = true;
		hideBox.Reset();
		currentAnim = &hideBox;
		if (!isHidded)
			currentAnim->speed = BOX_ANIM_SPEED;
		else
		{
			currentAnim->speed = -BOX_ANIM_SPEED;
			hideBox.currentFrame = hideBox.frames.size() - 1;
		}
		rect = currentAnim->GetCurrentFrame(dt);
		return;
	}

	if (currentAnim == &hideBox)
	{
		if (hideBox.IsFinished())
		{
			if (currentAnim->speed == BOX_ANIM_SPEED)
			{
				isHidded = true;
				currentAnim = &walkBox;
			}
			else
			{
				isHidded = false;
				currentAnim = &idleLeft;
			}
			
		}
		else
		{
			rect = currentAnim->GetCurrentFrame(dt);
			return;
		}
	}

	fPoint newDir = fPoint(0.0f, 0.0f);
	if (g_Input->GetKey(SDL_SCANCODE_D))
	{
		newDir.x = 1.0f;
		currentAnim = &walkRight;
	}
	else if (g_Input->GetKey(SDL_SCANCODE_A))
	{
		newDir.x = -1.0f;
		currentAnim = &walkLeft;
	}
	else if (g_Input->GetKey(SDL_SCANCODE_S))
	{
		newDir.y = 1.0f;
		currentAnim = &walkDown;
	}
	else if (g_Input->GetKey(SDL_SCANCODE_W))
	{
		newDir.y = -1.0f;
		currentAnim = &walkUp;
	}
	
	isStop = (newDir.x == 0.0f && newDir.y == 0.0f) ? true : false;
	
	if (isStop && (dir.x != 0.0f || dir.y != 0.0f))
	{
		if (dir.x == 1.0f) currentAnim = &idleRight;
		if (dir.x == -1.0f) currentAnim = &idleLeft;
		if (dir.y == 1.0f) currentAnim = &idleDown;
		if (dir.y == -1.0f) currentAnim = &idleUp;
	}

	if (isHidded)
	{
		currentAnim = &walkBox;
		currentAnim->speed = isStop ? 0.0f : BOX_ANIM_SPEED;
	}

	dir = newDir;

	rect = currentAnim->GetCurrentFrame(dt);
}

void Player::InitAnimations()
{
	// Idle
	idleDown.frames.push_back({	0, 0, 16, 27 });
	idleDown.frames.push_back({ 16, 0, 16, 27 });
	idleDown.frames.push_back({ 0, 27, 16, 27 });
	idleDown.frames.push_back({ 16, 27, 16, 27 });
	idleDown.speed = 10.0f;

	idleLeft.frames.push_back({ 32,  0, 17, 27 });
	idleLeft.frames.push_back({ 49,  0, 17, 27 });
	idleLeft.frames.push_back({ 32, 27, 17, 27 });
	idleLeft.frames.push_back({ 49, 27, 17, 27 });
	idleLeft.speed = 8.0f;

	idleRight.frames.push_back({ 66,  0, 17, 27 });
	idleRight.frames.push_back({ 83,  0, 17, 27 });
	idleRight.frames.push_back({ 66, 27, 17, 27 });
	idleRight.frames.push_back({ 83, 27, 17, 27 });
	idleRight.speed = 8.0f;

	idleUp.frames.push_back({ 100,  0, 16, 27 });
	idleUp.frames.push_back({ 116,  0, 16, 27 });
	idleUp.frames.push_back({ 100, 27, 16, 27 });
	idleUp.frames.push_back({ 116, 27, 16, 27 });
	idleUp.speed = 10.0f;

	// Walk
	walkDown.frames.push_back({	 0, 54, 16, 28 });
	walkDown.frames.push_back({ 16, 54, 16, 28 });
	walkDown.frames.push_back({ 32, 54, 16, 28 });
	walkDown.frames.push_back({  0, 82, 16, 28 });
	walkDown.frames.push_back({ 16, 82, 16, 28 });
	walkDown.frames.push_back({ 32, 82, 16, 28 });
	walkDown.speed = 10.0f;

	walkLeft.frames.push_back({ 0 , 110, 17, 26 });
	walkLeft.frames.push_back({ 17, 110, 17, 26 });
	walkLeft.frames.push_back({ 34, 110, 17, 26 });
	walkLeft.frames.push_back({ 0, 136, 17, 26 });
	walkLeft.frames.push_back({ 17, 136, 17, 26 });
	walkLeft.frames.push_back({ 34, 136, 17, 26 });
	walkLeft.speed = 8.0f;

	walkRight.frames.push_back({ 51, 110, 17, 26 });
	walkRight.frames.push_back({ 68, 110, 17, 26 });
	walkRight.frames.push_back({ 85, 110, 17, 26 });
	walkRight.frames.push_back({ 51, 136, 17, 26 });
	walkRight.frames.push_back({ 68, 136, 17, 26 });
	walkRight.frames.push_back({ 85, 136, 17, 26 });
	walkRight.speed = 8.0f;

	walkUp.frames.push_back({ 48, 54, 16, 28 });
	walkUp.frames.push_back({ 64, 54, 16, 28 });
	walkUp.frames.push_back({ 80, 54, 16, 28 });
	walkUp.frames.push_back({ 48, 82, 16, 28 });
	walkUp.frames.push_back({ 64, 82, 16, 28 });
	walkUp.frames.push_back({ 80, 82, 16, 28 });
	walkUp.speed = 10.0f;

	// Box
	hideBox.frames.push_back({ 0, 162, 29, 38});
	hideBox.frames.push_back({ 29, 162, 29, 38 });
	hideBox.frames.push_back({ 58, 162, 29, 38 });
	hideBox.frames.push_back({ 87, 162, 29, 38 });
	hideBox.frames.push_back({ 0, 200, 29, 38 });
	hideBox.speed = 10.0f;
	hideBox.looping = false;

	walkBox.frames.push_back({0, 238, 29, 21});
	walkBox.frames.push_back({ 29, 238, 29, 21 });
	walkBox.frames.push_back({ 58, 238, 29, 21 });
	walkBox.frames.push_back({ 87, 238, 29, 21 });
	walkBox.speed = 10.0f;
}
