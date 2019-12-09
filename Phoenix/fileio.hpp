#pragma once
//-----
#include <stack>
#include <string>
#include <fstream>
#include <algorithm>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include "input.hpp"
//----
//---
//--
namespace FileIO
{
	template <class T>
	class FormatNumArray4Out
	{
		public:
			FormatNumArray4Out(std::basic_ostream<TCHAR> & out):
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

			static inline void pushTag(std::basic_string<TCHAR> tag)
			{
				FileIO::Export::xmlTags.push(tag);
			}

			static inline void popTag()
			{
				FileIO::Export::xmlTags.pop();
			}

			static inline void topOpen(std::basic_ostream<TCHAR> & out)
			{
				out << std::endl << _T("<") << FileIO::Export::xmlTags.top() << _T(">");
			}

			static inline void topClose(std::basic_ostream<TCHAR> & out)
			{
				out << _T("</") << FileIO::Export::xmlTags.top() << _T(">");
			}

		private:
			static bool poped;
			static std::stack<std::basic_string<TCHAR>> xmlTags;
	};

	class Import
	{
		public:
			static bool isTag(_IN_(std::basic_string<TCHAR> & tag), _IN_(rapidxml::xml_node<TCHAR>* node));
	};
}