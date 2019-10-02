#include "rcomboboxhandler.hpp"

RComboBoxHandler::RComboBoxHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(std::vector<Property> & comboboxContent), _IN_(USHORT & selected)):
RGalleryControlHandler(hWnd, enabled, comboboxContent),
selected(selected)
{
}

RComboBoxHandler::~RComboBoxHandler()
{
}

STDMETHODIMP RComboBoxHandler::UpdateProperty(UINT nCmdID, _IN_(PROPERTYKEY & key), _IN_(PROPVARIANT* pPropvarCurrentValue), _OUT_(PROPVARIANT* pPropvarNewValue))
{
	HRESULT hResult = RGalleryControlHandler::UpdateProperty(nCmdID, key, pPropvarCurrentValue, pPropvarNewValue);

	UNREFERENCED_PARAMETER(nCmdID);
	UNREFERENCED_PARAMETER(pPropvarCurrentValue);

	if(hResult == E_NOTIMPL)
	{
		if(key == UI_PKEY_SelectedItem)
		{
			hResult = UIInitPropertyFromUInt32(key, this->selected, pPropvarNewValue);
		}
	}

	return hResult;
}

STDMETHODIMP RComboBoxHandler::Execute(UINT nCmdID, UI_EXECUTIONVERB verb, _IN_(PROPERTYKEY* key), _IN_(PROPVARIANT* pPropvarValue), IUISimplePropertySet* pCommandExecutionProperties)
{
	UNREFERENCED_PARAMETER(nCmdID);
	UNREFERENCED_PARAMETER(pCommandExecutionProperties);

	HRESULT hResult = E_NOTIMPL;

	switch(verb)
	{
		case UI_EXECUTIONVERB_EXECUTE:
			{
				if(*key == UI_PKEY_SelectedItem)
				{
					UINT value;

					hResult = UIPropertyToUInt32(*key, *pPropvarValue, &value);
					IFFAILEDRETURN(hResult)

					if(value != UI_COLLECTION_INVALIDINDEX)
					{
						hResult = SendMessage(this->hWnd, WM_RIBBON, nCmdID, value);
					}
					else
					{
						hResult = SendMessage(this->hWnd, WM_RIBBON, nCmdID, STRINGVALUE);
					}
				}
			}
			break;
	}

	return hResult;
}