#include "input.hpp"

void Input::list2floatArray(_IN_(std::basic_string<TCHAR> & list), _IN_(std::basic_string<TCHAR> & delimiter), _OUT_(std::vector<float> & vector))
{
	PTCHAR token = nullptr;
	PTCHAR nextToken = nullptr;
	std::basic_string<TCHAR> tmpList = list.substr(0, Input::maxStringSize);

	size_t vectorSize = vector.size();

	if(vectorSize > 0)
	{
		std::fill(vector.begin(), vector.end(), 0.0f);
	}

	if(!list.empty() && !delimiter.empty())
	{
		token = _tcstok_s(&tmpList.front(), delimiter.data(), &nextToken);

		for(unsigned char i = 0; token != nullptr; i++)
		{
			if(vectorSize > 0)
			{
				if(vectorSize == i)
				{
					break;
				}

				vector[i] = _tcstof(token, nullptr);
			}
			else
			{
				ExceptionHandler::push_back<float>(vector, _tcstof(token, nullptr));
			}

			token = _tcstok_s(nullptr, delimiter.data(), &nextToken);
		}
	}
}

bool Input::checkFloatArrayValues(_IN_(std::vector<float> & vector), const std::vector<std::array<float, 2>> & valuesMinMax)
{
	if(vector.size() > valuesMinMax.size())
	{
		return false;
	}
	else
	{
		std::vector<std::array<float, 2>>::const_iterator k = valuesMinMax.begin();
		for(std::vector<float>::const_iterator i = vector.begin(), j = vector.end(); i != j; i++, k++)
		{
			if(*i < (*k)[0] || *i > (*k)[1])
			{
				return false;
			}
		}

		return true;
	}
}