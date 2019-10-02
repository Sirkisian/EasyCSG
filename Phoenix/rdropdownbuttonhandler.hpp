#pragma once
//-----
#include "rcontrolhandler.hpp"
//----
//---
//--

//Drop-Down Button
class RDropDownButtonHandler : public RControlHandler
{
	public:
		static inline HRESULT CreateInstance(_OUT_(IUICommandHandler** ppCommandHandler), _IN_(BOOL & enabled))
		{
			return RibbonFunctions::CreateInstance(reinterpret_cast<IUnknown**>(ppCommandHandler), new RDropDownButtonHandler(enabled));
		}

		virtual ~RDropDownButtonHandler();

	private:
		RDropDownButtonHandler(_IN_(BOOL & enabled));
};