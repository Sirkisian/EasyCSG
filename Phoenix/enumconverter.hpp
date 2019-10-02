#pragma once
//-----
//----
//---
//--
namespace EnumConverter
{
	template <class T>
	T convert(unsigned char value, T minValue, T maxValue)
	{
		unsigned char min = static_cast<unsigned char>(minValue);
		unsigned char max = static_cast<unsigned char>(maxValue);

		if(value >= min && value <= max)
		{
			return static_cast<T>(value);
		}
		else
		{
			return static_cast<T>(0);
		}
	}
};