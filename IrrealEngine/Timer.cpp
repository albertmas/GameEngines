// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	reset = false;
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Reset()
{
	reset = true;
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if (!reset)
	{
		if (running == true)
			return SDL_GetTicks() - started_at;
		else
			return 0;
	}
	else
		return 0;
}

// ---------------------------------------------
float Timer::ReadSec() const
{
	if (!reset)
	{
		if (running == true)
			return float(SDL_GetTicks() - started_at) / 1000.0f;
		else
			return 0;
	}
	else
		return 0;
}