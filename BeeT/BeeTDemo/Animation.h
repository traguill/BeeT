#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL.h"

#include <vector>

using namespace std;

class Animation
{
public:
	Animation();
	~Animation();

	SDL_Rect& GetCurrentFrame(float deltaTime);
	bool IsFinished()const;
	void Reset();
public:
	vector<SDL_Rect> frames;
	float speed = 1.0f;
	bool looping = true;
	float currentFrame = 0.0f;
private:
	int numLoops = 0;
};
#endif // !__ANIMATION_H__
