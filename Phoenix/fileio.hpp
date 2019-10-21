#pragma once
//-----
#include <stack>
#include <string>
#include <fstream>
#include <algorithm>
#include <tchar.h>
#include "macros.hpp"
//----
//---
//--
namespace FileIO
{
	template <class T>
	class FormatNumber4Out
	{
		public:
			FormatNumber4Out(std::basic_ostream<TCHAR> & out):
			first(true),
			out(out)
			{
			}

			void operator()(_IN_(T & i))
			{
				if(first)
				{
					this->out << std::fixed;
				}
				else
				{
					this->out << _T(" ");
				}

				this->out << i;

				if(this->first)
				{
					this->first = false;
				}
			}

		private:
			bool first;
			std::basic_ostream<TCHAR> & out;
	};

	class Export
	{
		public:
			static void push(std::basic_ostream<TCHAR> & out, std::basic_string<TCHAR> tag);
			static std::basic_ostream<TCHAR> & pop(std::basic_ostream<TCHAR> & out);

		private:
			static bool poped;
			static std::stack<std::basic_string<TCHAR>> xmlTags;
	};
}