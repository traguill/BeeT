#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Animation.h"

class Player : public Entity
{
public:
	Player(SDL_Renderer* renderer, float posX, float posY);
	~Player();

	void UpdateLogic(float dt);

private:
	void InitAnimations();

public:
	bool isHidded = false;
private:
	Animation idleDown;
	Animation idleUp;
	Animation idleRight;
	Animation idleLeft;

	Animation walkDown;
	Animation walkUp;
	Animation walkRight;
	Animation walkLeft;

	Animation hideBox;
	Animation walkBox;

	Animation* currentAnim;

};
#endif // !__PLAYER_H__
