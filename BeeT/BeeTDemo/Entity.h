#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL/include/SDL.h"

class Entity
{
public:
	Entity(float posX, float posY);
	~Entity();

	bool LoadSprite(SDL_Renderer* renderer, const char* path, int sizeX, int sizeY);
	
	void Tick(float dt);
	void Draw(SDL_Renderer* renderer);

	virtual void UpdateLogic(float dt);

private:
	void Move(float dt);

public:
	// Position
	float posX = 0.0f;
	float posY = 0.0f;
	// Rotation
	float rotX = 0.0f;
	float rotY = 0.0f;
	// Speed & Direction
	float speed = 0.0f;
	float dirX = 0.0f;
	float dirY = 0.0f;

	double angle = 0.0;

private:
	// Render
	SDL_Rect rect;
	SDL_Texture* texture = NULL;
};
#endif // !__ENTITY_H__
