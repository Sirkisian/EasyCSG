#pragma once
//-----
#include <windows.h>
//----
//---
//--
class Timer
{
	public:
		Timer(BYTE id, UINT limit);
		~Timer();

		VOID setTimer(HWND hWnd);
		VOID incrementTimer();
		VOID killTimer();

		inline BOOL valid()
		{
			return this->hWnd != NULL && this->timer < this->limit;
		}

		const BYTE id;

	private:
		const UINT limit;

		HWND hWnd;
		UINT timer;
};