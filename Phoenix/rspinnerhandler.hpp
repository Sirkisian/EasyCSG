#pragma once
//-----
#include "rcontrolhandler.hpp"
//----
//---
//--

//Spinner
class RSpinnerHandler : public RControlHandler
{
	public:
		static inline HRESULT CreateInstance(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, _IN_(BOOL & enabled), _IN_(FLOAT & minValue), _IN_(FLOAT & maxValue), _IN_(FLOAT & increment))
		{
			return RibbonFunctions::CreateInstance(reinterpret_cast<IUnknown**>(ppCommandHandler), new RSpinnerHandler(hWnd, enabled, minValue, maxValue, increment));
		}

		virtual ~RSpinnerHandler();

		STDMETHOD(UpdateProperty)(UINT nCmdID, _IN_(PROPERTYKEY & key), _IN_(PROPVARIANT* pPropvarCurrentValue), _OUT_(PROPVARIANT* pPropvarNewValue));
		STDMETHOD(Execute)(UINT nCmdID, UI_EXECUTIONVERB verb, _IN_(PROPERTYKEY* key), _IN_(PROPVARIANT* pPropvarValue), IUISimplePropertySet* pCommandExecutionProperties);

	private:
		RSpinnerHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(FLOAT & minValue), _IN_(FLOAT & maxValue), _IN_(FLOAT & increment));

		//purpose - sets the spinner value
		//body - 
		//in:
		//	param(1) -
		//	param(2) -
		//	param(3) -
		//out:
		//	
		HRESULT SetSpinnerValue(_OUT_(PROPVARIANT* pPropvaValue), _IN_(PROPERTYKEY & key), _IN_(FLOAT & value));

		FLOAT minValue;
		FLOAT maxValue;
		FLOAT increment;
};