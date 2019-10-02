#include "rspinnerhandler.hpp"

RSpinnerHandler::RSpinnerHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(FLOAT & minValue), _IN_(FLOAT & maxValue), _IN_(FLOAT & increment)):
RControlHandler(hWnd, enabled),
minValue(minValue),
maxValue(maxValue),
increment(increment)
{
}

RSpinnerHandler::~RSpinnerHandler()
{
}

STDMETHODIMP RSpinnerHandler::UpdateProperty(UINT nCmdID, _IN_(PROPERTYKEY & key), _IN_(PROPVARIANT* pPropvarCurrentValue), _OUT_(PROPVARIANT* pPropvarNewValue))
{
	HRESULT hResult = RControlHandler::UpdateProperty(nCmdID, key, pPropvarCurrentValue, pPropvarNewValue);

	UNREFERENCED_PARAMETER(nCmdID);
	UNREFERENCED_PARAMETER(pPropvarCurrentValue);

	if(hResult == E_NOTIMPL)
	{
		if(key == UI_PKEY_MinValue)
		{
			hResult = this->SetSpinnerValue(pPropvarNewValue, key, this->minValue);
		}
		else if(key == UI_PKEY_MaxValue)
		{
			hResult = this->SetSpinnerValue(pPropvarNewValue, key, this->maxValue);
		}
		else if(key == UI_PKEY_Increment)
		{
			hResult = this->SetSpinnerValue(pPropvarNewValue, key, this->increment);
		}
	}

	return hResult;
}

STDMETHODIMP RSpinnerHandler::Execute(UINT nCmdID, UI_EXECUTIONVERB verb, _IN_(PROPERTYKEY* key), _IN_(PROPVARIANT* pPropvarValue), IUISimplePropertySet* pCommandExecutionProperties)
{
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(pCommandExecutionProperties);

	HRESULT hResult = E_NOTIMPL;

	switch(verb)
	{
		case UI_EXECUTIONVERB_EXECUTE:
		{
			hResult = SendMessage(this->hWnd, WM_RIBBON, nCmdID, pPropvarValue->intVal);
		}
		break;
	}

	return hResult;
}

HRESULT RSpinnerHandler::SetSpinnerValue(_OUT_(PROPVARIANT* pPropvaValue), _IN_(PROPERTYKEY & key), _IN_(FLOAT & value))
{
	DECIMAL decimal;

	HRESULT hResult = VarDecFromR4(value, &decimal);
	IFFAILEDRETURN(hResult);

	return UIInitPropertyFromDecimal(key, decimal, pPropvaValue);
}