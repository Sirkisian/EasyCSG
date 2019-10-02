#pragma once
//-----
#include "rtogglecontrolhandler.hpp"
//----
//---
//--

//Toggle Button
class RToggleButtonHandler : public RToggleControlHandler
{
	public:
		static inline HRESULT CreateInstance(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, _IN_(BOOL & enabled), _IN_(BOOL & defValue))
		{
			return RibbonFunctions::CreateInstance(reinterpret_cast<IUnknown**>(ppCommandHandler), new RToggleButtonHandler(hWnd, enabled, defValue));
		}

		virtual ~RToggleButtonHandler();

	private:
		RToggleButtonHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(BOOL & defValue));
};