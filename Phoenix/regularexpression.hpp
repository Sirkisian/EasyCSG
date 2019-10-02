#pragma once
//-----
#include <regex>
#include <tchar.h>
#include "macros.hpp"
//----
//---
//--
class RegularExpression
{
	public:
		//purpose -
		//body - compare string param(1) with regular expression param(2)
		//in:
		//	param(1) - pointer to the string
		//	param(2) - pointer to the string with regular expression
		//out:
		//	match true, else false
		static bool match(_IN_(std::basic_string<TCHAR> & string), std::basic_string<TCHAR> regexp);
};