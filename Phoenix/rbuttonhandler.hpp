#pragma once
//-----
#include "rcontrolhandler.hpp"
//----
//---
//--

//Button
class RButtonHandler : public RControlHandler
{
	public:
		static inline HRESULT CreateInstance(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, _IN_(BOOL & enabled))
		{
			return RibbonFunctions::CreateInstance(reinterpret_cast<IUnknown**>(ppCommandHandler), new RButtonHandler(hWnd, enabled));
		}

		virtual ~RButtonHandler();

		STDMETHOD(Execute)(UINT nCmdID, UI_EXECUTIONVERB verb, _IN_(PROPERTYKEY* key), _IN_(PROPVARIANT* pPropvarValue), IUISimplePropertySet* pCommandExecutionProperties);
		
	private:
		RButtonHandler(HWND hWnd, _IN_(BOOL & enabled));
};