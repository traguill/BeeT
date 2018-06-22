#include "Timer.h"
#include <fstream>

Timer::Timer()
{
	frequency = SDL_GetPerformanceFrequency();	
}

Timer::~Timer()
{
}

void Timer::Start()
{
	timePaused = 0;
	timeStart = SDL_GetPerformanceCounter();
}

void Timer::Pause()
{
	timePauseStart = SDL_GetPerformanceCounter();
}

void Timer::Resume()
{
	timePaused += SDL_GetPerformanceCounter() - timePauseStart;
}

void Timer::End()
{
	double ms = (double(SDL_GetPerformanceCounter() - timeStart - timePaused) / double(frequency)) * 1000;
	samples.push_back(ms);
}

void Timer::Save()
{
	
	std::ofstream out("benchmarks2000.csv");
	if (!out)
		return;
	for (auto s : samples)
	{
		out << s << "," << std::endl;
	}
	out.close();
}
