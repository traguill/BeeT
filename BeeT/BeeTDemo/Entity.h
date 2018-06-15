#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL/include/SDL.h"
#include "GameManager.h"
#include "Math2D.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

enum CLASS_TYPE
{
	PLAYER,
	ENEMY,
	PLAYER_BULLET,
	ENEMY_BULLET,
	BLOCK
};

class Entity
{
public:
	Entity(SDL_Renderer* renderer, float posX, float posY);
	~Entity();
	
	void Tick(float dt);
	void Draw();

	virtual void OnCollision(Entity* otherEntity);

protected:
	bool LoadSprite(const char* path, int sizeX, int sizeY);
	virtual void UpdateLogic(float dt);

private:
	void Move(float dt);

public:
	// Position
	fPoint pos;
	
	// Rotation
	fVec rot;
	// Speed & Direction
	float speed = 0.0f;
	fVec dir;

	double angle = 0.0;

	bool hasCollider = false;
	CLASS_TYPE type;

protected:
	// Render
	SDL_Renderer* renderer;
	SDL_Rect rect;
	SDL_Texture* texture = NULL;

};
#endif // !__ENTITY_H__
