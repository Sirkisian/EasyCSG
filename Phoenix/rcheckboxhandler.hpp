#pragma once
//-----
#include "rtogglecontrolhandler.hpp"
//----
//---
//--

//Check Box
class RCheckBoxHandler : public RToggleControlHandler
{
	public:
		static inline HRESULT CreateInstance(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, _IN_(BOOL & enabled), _IN_(BOOL & defValue))
		{
			return RibbonFunctions::CreateInstance(reinterpret_cast<IUnknown**>(ppCommandHandler), new RCheckBoxHandler(hWnd, enabled, defValue));
		}

		virtual ~RCheckBoxHandler();

	private:
		RCheckBoxHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(BOOL & defValue));
};