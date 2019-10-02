#pragma once
//-----
#include <array>
#include "macros.hpp"
//----
//---
//--
class Mouse
{
	public:
		Mouse()
		{
			this->clear();
		}

		~Mouse()
		{
		}

		void clear()
		{
			this->positionDefault[mCOORDINATE::X] = this->positionMove[mCOORDINATE::X] = this->sign[mCOORDINATE::X] = 0;
			this->positionDefault[mCOORDINATE::Y] = this->positionMove[mCOORDINATE::Y] = this->sign[mCOORDINATE::Y] = 0;
		}

		void set(short x, short y)
		{
			this->positionMove[mCOORDINATE::X] = this->positionDefault[mCOORDINATE::X] = x;
			this->positionMove[mCOORDINATE::Y] = this->positionDefault[mCOORDINATE::Y] = y;
		}

		void move(short x, short y)
		{
			this->sign[mCOORDINATE::X] = this->sign[mCOORDINATE::Y] = 0;

			if(this->positionMove[mCOORDINATE::X] != x)
			{
				this->sign[mCOORDINATE::X] = this->positionMove[mCOORDINATE::X] > x ? 1 : -1;
			}

			if(this->positionMove[mCOORDINATE::Y] != y)
			{
				this->sign[mCOORDINATE::Y] = this->positionMove[mCOORDINATE::Y] > y ? 1 : -1;
			}

			this->positionMove[mCOORDINATE::X] = x;
			this->positionMove[mCOORDINATE::Y] = y;
		}

		std::array<char, 2> sign;

	private:
		std::array<short, 2> positionMove;
		std::array<short, 2> positionDefault;
};