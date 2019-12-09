#pragma once
//-----
#include <array>
#include <vector>
#include <sstream>
#include <tchar.h>
#include "macros.hpp"
#include "exceptionhandler.hpp"
//----
//---
//--
class Input
{
	public:
		template <class T>
		static void list2Num(_IN_(std::basic_string<TCHAR> & list), TCHAR delimiter, _OUT_(std::vector<T> & vector))
		{
			std::basic_string<TCHAR> string;
			std::basic_istringstream<TCHAR> values(list);

			vector.clear();

			while(std::getline(values, string, delimiter))
			{
				ExceptionHandler::push_back<T>(vector, Input::convert<T>(string));
			}
		}

		template <class T> //std::vector or std::array
		static void list2NumArray(_IN_(std::basic_string<TCHAR> & list), TCHAR delimiter, _OUT_(T & array))
		{
			std::basic_string<TCHAR> string;
			std::basic_istringstream<TCHAR> values(list);

			std::fill(array.begin(), array.end(), static_cast<typename T::value_type>(0));

			for(size_t i = 0, j = array.size(); std::getline(values, string, delimiter); i++)
			{
				if(i < j)
				{
					array[i] = Input::convert<T::value_type>(string);
				}
				else
				{
					break;
				}
			}
		}

		template <class T> //std::vector or std::array
		static bool checkFloatArrayValues(_IN_(T & array), const std::vector<std::array<float, 2>> & valuesMinMax)
		{
			if(array.size() > valuesMinMax.size())
			{
				return false;
			}
			else
			{
				std::vector<std::array<float, 2>>::const_iterator k = valuesMinMax.begin();
				for(typename T::const_iterator i = array.begin(), j = array.end(); i != j; i++, k++)
				{
					if(*i < (*k)[0] || *i > (*k)[1])
					{
						return false;
					}
				}

				return true;
			}
		}

	private:
		template <class T>
		static inline T convert(_IN_(std::basic_string<TCHAR> & string))
		{
		}

		template <>
		static inline unsigned int convert<unsigned int>(_IN_(std::basic_string<TCHAR> & string))
		{
			return _ttoi(string.c_str());
		}

		template <>
		static inline float convert<float>(_IN_(std::basic_string<TCHAR> & string))
		{
			return _tcstof(string.c_str(), nullptr);
		}
};