#include "Animation.h"

Animation::Animation()
{}

Animation::~Animation()
{}

SDL_Rect & Animation::GetCurrentFrame(float deltaTime)
{
	currentFrame += speed * deltaTime;
	if (currentFrame >= frames.size())
	{
		currentFrame = looping ? 0.0f : frames.size() - 1;
		numLoops++;
	}
	else if (currentFrame < 0)
	{
		currentFrame = looping ? frames.size() - 1 : 0.0f;
		numLoops++;
	}
	return frames[(int)currentFrame];
}

bool Animation::IsFinished() const
{
	return numLoops > 0;
}

void Animation::Reset()
{
	numLoops = 0;
	currentFrame = 0.0f;
}

