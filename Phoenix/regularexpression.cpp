#include "regularexpression.hpp"

bool RegularExpression::match(_IN_(std::basic_string<TCHAR> & string), std::basic_string<TCHAR> regexp)
{
	std::basic_regex<TCHAR> regExpression;

	try
	{
		regExpression = regexp;
	}
	catch(const std::regex_error &)
	{
		return false;
	}

	return std::regex_match(string, regExpression);
};