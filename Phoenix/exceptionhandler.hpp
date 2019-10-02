#pragma once
//-----
#include <vector>
//----
//---
//--
namespace ExceptionHandler
{
	template<class T>
	bool push_back(std::vector<T> & object, T pushObject)
	{
		try
		{
			object.push_back(pushObject);

			return true;
		}
		catch(const std::exception &)
		{
		}
		catch(...)
		{
		}

		return false;
	}
}