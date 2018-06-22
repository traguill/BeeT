#ifndef __TIMER_H__
#define __TIMER_H__

#include "SDL/include/SDL_timer.h"
#include <vector>
class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	void Pause();
	void Resume();
	void End();
	void Save();

private:
	Uint64 frequency, timeStart, timePauseStart, timePaused;
	std::vector<double> samples;
};
extern Timer* g_Timer;
#endif // !__TIMER_H__
