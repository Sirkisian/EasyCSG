#pragma once
//-----
#include <array>
#include <vector>
#include <tchar.h>
#include "macros.hpp"
#include "exceptionhandler.hpp"
//----
//---
//--
class Input
{
	public:
		static const unsigned char maxStringSize = 255;

		static void list2floatArray(_IN_(std::basic_string<TCHAR> & list), _IN_(std::basic_string<TCHAR> & delimiter), _OUT_(std::vector<float> & vector));

		static bool checkFloatArrayValues(_IN_(std::vector<float> & vector), const std::vector<std::array<float, 2>> & valuesMinMax);
};