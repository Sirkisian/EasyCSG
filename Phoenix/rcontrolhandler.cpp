#include "rcontrolhandler.hpp"

RControlHandler::RControlHandler(HWND hWnd, _IN_(BOOL & enabled)):
cRef(1), //set to 1!
hWnd(hWnd),
enabled(enabled)
{
}

RControlHandler::~RControlHandler()
{
}

STDMETHODIMP_(ULONG) RControlHandler::Release()
{
	LONG cRef = InterlockedDecrement(&this->cRef);

	if(cRef == 0)
	{
		delete this;
	}

	return cRef;
}

STDMETHODIMP RControlHandler::QueryInterface(_IN_(IID & iid), _OUT_(VOID** ppv))
{
	HRESULT hResult = RibbonFunctions::QueryInterface(iid, ppv, *this);

	if(SUCCEEDED(hResult))
	{
		this->AddRef();
	}

	return hResult;
}

STDMETHODIMP RControlHandler::UpdateProperty(UINT nCmdID, _IN_(PROPERTYKEY & key), _IN_(PROPVARIANT* pPropvarCurrentValue), _OUT_(PROPVARIANT* pPropvarNewValue))
{
	UNREFERENCED_PARAMETER(nCmdID);

	HRESULT hResult = E_NOTIMPL;

	if(key == UI_PKEY_Enabled)
	{
		BOOL enabled;

		hResult = UIPropertyToBoolean(key, *pPropvarCurrentValue, &enabled);

		if(SUCCEEDED(hResult))
		{
			if(this->enabled != enabled)
			{
				hResult = UIInitPropertyFromBoolean(key, this->enabled, pPropvarNewValue);
			}
		}
	}

	return hResult;
}

STDMETHODIMP RControlHandler::Execute(UINT nCmdID, UI_EXECUTIONVERB verb, _IN_(PROPERTYKEY* key), _IN_(PROPVARIANT* pPropvarValue), IUISimplePropertySet* pCommandExecutionProperties)
{
	UNREFERENCED_PARAMETER(nCmdID);
	UNREFERENCED_PARAMETER(verb);
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(pPropvarValue);
	UNREFERENCED_PARAMETER(pCommandExecutionProperties);

	return E_NOTIMPL;
}