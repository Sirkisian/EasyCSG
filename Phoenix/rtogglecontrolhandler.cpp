#include "rtogglecontrolhandler.hpp"

RToggleControlHandler::RToggleControlHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(BOOL & defValue)):
RControlHandler(hWnd, enabled),
defValue(defValue)
{
}

RToggleControlHandler::~RToggleControlHandler()
{
}

STDMETHODIMP RToggleControlHandler::UpdateProperty(UINT nCmdID, _IN_(PROPERTYKEY & key), _IN_(PROPVARIANT* pPropvarCurrentValue), _OUT_(PROPVARIANT* pPropvarNewValue))
{
	HRESULT hResult = RControlHandler::UpdateProperty(nCmdID, key, pPropvarCurrentValue, pPropvarNewValue);

	UNREFERENCED_PARAMETER(nCmdID);
	UNREFERENCED_PARAMETER(pPropvarCurrentValue);

	if(hResult == E_NOTIMPL) 
	{
		if(key == UI_PKEY_BooleanValue)
		{
			hResult = UIInitPropertyFromBoolean(key, this->defValue, pPropvarNewValue);
		}
	}

	return hResult;
}

STDMETHODIMP RToggleControlHandler::Execute(UINT nCmdID, UI_EXECUTIONVERB verb, _IN_(PROPERTYKEY* key), _IN_(PROPVARIANT* pPropvarValue), IUISimplePropertySet* pCommandExecutionProperties)
{
	UNREFERENCED_PARAMETER(verb);
	UNREFERENCED_PARAMETER(pCommandExecutionProperties);

	HRESULT hResult = E_NOTIMPL;

	if(*key == UI_PKEY_BooleanValue)
	{
		BOOL value;

		hResult = UIPropertyToBoolean(*key, *pPropvarValue, &value);

		if(SUCCEEDED(hResult))
		{
			hResult = SendMessage(this->hWnd, WM_RIBBON, nCmdID, value);
		}
	}

	return hResult;
}