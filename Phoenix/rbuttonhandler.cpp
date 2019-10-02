#include "rbuttonhandler.hpp"

RButtonHandler::RButtonHandler(HWND hWnd, _IN_(BOOL & enabled)):
RControlHandler(hWnd, enabled)
{
}

RButtonHandler::~RButtonHandler()
{
}

STDMETHODIMP RButtonHandler::Execute(UINT nCmdID, UI_EXECUTIONVERB verb, _IN_(PROPERTYKEY* key), _IN_(PROPVARIANT* pPropvarValue), IUISimplePropertySet* pCommandExecutionProperties)
{
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(pPropvarValue);
	UNREFERENCED_PARAMETER(pCommandExecutionProperties);

	HRESULT hResult = E_NOTIMPL;

	switch(verb)
	{
		case UI_EXECUTIONVERB_EXECUTE:
		{
			hResult = SendMessage(this->hWnd, WM_RIBBON, nCmdID, 0);
		}
		break;
	}

	return hResult;
}