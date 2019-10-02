#include "timer.hpp"

Timer::Timer(BYTE id, UINT limit):
id(id),
limit(limit),
hWnd(NULL),
timer(0)
{
}

Timer::~Timer()
{
	if(this->hWnd != NULL)
	{
		this->killTimer();
	}
}

VOID Timer::setTimer(HWND hWnd)
{
	if(this->timer > 0)
	{
		this->timer = 0;
	}
	else
	{
		if(SetTimer(hWnd, this->id, 1000, NULL) > 0)
		{
			this->hWnd = hWnd;
		}
	}
}

VOID Timer::incrementTimer()
{
	if(this->hWnd != NULL)
	{
		this->timer++;
	}
}

VOID Timer::killTimer()
{
	if(KillTimer(this->hWnd, this->id))
	{
		this->hWnd = NULL;
		this->timer = 0;
	}
}