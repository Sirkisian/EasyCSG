#pragma once
//-----
#include "rcontrolhandler.hpp"
//----
//---
//--

//abstract control class for controls with toggle state as Check Box and Toggle Button
class RToggleControlHandler : public RControlHandler
{
	public: 
		RToggleControlHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(BOOL & defValue));
		virtual ~RToggleControlHandler() = 0;

		STDMETHOD(UpdateProperty)(UINT nCmdID, _IN_(PROPERTYKEY & key), _IN_(PROPVARIANT* pPropvarCurrentValue), _OUT_(PROPVARIANT* pPropvarNewValue));
		STDMETHOD(Execute)(UINT nCmdID, UI_EXECUTIONVERB verb, _IN_(PROPERTYKEY* key), _IN_(PROPVARIANT* pPropvarValue), IUISimplePropertySet* pCommandExecutionProperties);

	protected:
		//default boolean state on create
		BOOL defValue;
};