#include "Random.h"
#include <chrono>

Random::Random()
{
	seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
}

float Random::RandomFloat()
{

	seed ^= (seed << 5);
	seed ^= (seed >> 13);
	seed ^= (seed << 6);

	return seed * (1.0f / 4294967295.0f);
}

int Random::RandomInt()
{
	seed ^= (seed << 5);
	seed ^= (seed >> 13);
	seed ^= (seed << 6);
	return (int)seed;
}

int Random::RandomInt(int min, int max)
{
	seed ^= (seed << 5);
	seed ^= (seed >> 13);
	seed ^= (seed << 6);
	float rnd =  seed * (1.0f / 4294967295.0f);

	return (int)(rnd * (float)(max - min) + min);
}

void Random::SetSeed(unsigned int seed)
{
	this->seed = seed;
}