#include "ribboncontrolmanager.hpp"

HRESULT RibbonControlManager::EnableDisableCommands(IUIFramework* framework, _IN_(std::vector<UINT> & commands), BOOL value)
{
	BOOL valueCurrent;
	PROPVARIANT propvarNew;
	PROPVARIANT propvarCurrent;

	if(framework == NULL)
	{
		return E_POINTER;
	}

	PropVariantInit(&propvarNew);
	PropVariantInit(&propvarCurrent);

	HRESULT hResult = UIInitPropertyFromBoolean(UI_PKEY_Enabled, value, &propvarNew);
	IFFAILEDRETURN(hResult);

	for(size_t i = 0, count = commands.size(); i < count; i++)
	{
		hResult = framework->GetUICommandProperty(commands[i], UI_PKEY_Enabled, &propvarCurrent);

		if(SUCCEEDED(hResult))
		{
			hResult = UIPropertyToBoolean(UI_PKEY_Enabled, propvarCurrent, &valueCurrent);
			IFFAILEDRETURN(hResult);

			if(value != valueCurrent)
			{
				hResult = framework->SetUICommandProperty(commands[i], UI_PKEY_Enabled, propvarNew);
				IFFAILEDRETURN(hResult);
			}
		}
	}

	PropVariantClear(&propvarCurrent);
	PropVariantClear(&propvarNew);

	return S_OK;
}

HRESULT RibbonControlManager::SetToggleButtonGroup(IUIFramework* framework, _IN_(std::vector<UINT> & commands), _IN_(UINT & command), BOOL radioButton)
{
	BOOL value;
	PROPVARIANT propvar;
	PROPVARIANT propvarFalse;

	if(framework == NULL)
	{
		return E_POINTER;
	}

	PropVariantInit(&propvar);
	PropVariantInit(&propvarFalse);

	HRESULT hResult = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, FALSE, &propvarFalse);
	IFFAILEDRETURN(hResult);

	for(size_t i = 0, count = commands.size(); i < count; i++)
	{
		hResult = framework->GetUICommandProperty(commands[i], UI_PKEY_BooleanValue, &propvar);
		IFFAILEDRETURN(hResult);

		hResult = UIPropertyToBoolean(UI_PKEY_BooleanValue, propvar, &value);
		IFFAILEDRETURN(hResult);

		if(commands[i] == command)
		{
			if(radioButton)
			{
				if(!value)
				{
					hResult = UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, TRUE, &propvar);
					IFFAILEDRETURN(hResult);

					hResult = framework->SetUICommandProperty(commands[i], UI_PKEY_BooleanValue, propvar);
					IFFAILEDRETURN(hResult);
				}
			}

			continue;
		}

		if(value)
		{
			hResult = framework->SetUICommandProperty(commands[i], UI_PKEY_BooleanValue, propvarFalse);
			IFFAILEDRETURN(hResult);
		}
	}

	PropVariantClear(&propvarFalse);
	PropVariantClear(&propvar);

	return hResult;
}

HRESULT RibbonControlManager::GetSelectedControl(IUIFramework* framework, _IN_(std::vector<UINT> & commands), _OUT_(UINT & command))
{
	BOOL value;
	HRESULT hResult;
	PROPVARIANT propvar;

	command = 0;

	if(framework == NULL)
	{
		return E_POINTER;
	}

	PropVariantInit(&propvar);

	for(size_t i = 0, count = commands.size(); i < count; i++)
	{
		hResult = framework->GetUICommandProperty(commands[i], UI_PKEY_BooleanValue, &propvar);
		IFFAILEDRETURN(hResult);

		hResult = UIPropertyToBoolean(UI_PKEY_Enabled, propvar, &value);
		IFFAILEDRETURN(hResult);

		if(value == TRUE)
		{
			command = commands[i];

			break;
		}
	}

	PropVariantClear(&propvar);

	return S_OK;
}

HRESULT RibbonControlManager::GetStringValue(IUIFramework* framework, UINT command, _OUT_(std::basic_string<TCHAR> & string))
{
	PROPVARIANT propvar;
	LPTSTR value = NULL;

	if(framework == NULL)
	{
		return E_POINTER;
	}

	PropVariantInit(&propvar);

	HRESULT hResult = framework->GetUICommandProperty(command, UI_PKEY_StringValue, &propvar);
	IFFAILEDRETURN(hResult);

	hResult = UIPropertyToStringAlloc(UI_PKEY_StringValue, propvar, &value);

	if(SUCCEEDED(hResult))
	{
		string = value;

		CoTaskMemFree(value);
	}

	PropVariantClear(&propvar);

	return hResult;
}

HRESULT RibbonControlManager::GetDecimalValue(IUIFramework* framework, UINT command, _OUT_(FLOAT & value))
{
	DECIMAL decimal;
	PROPVARIANT propvar;

	value = 0.0f;

	if(framework == NULL)
	{
		return E_POINTER;
	}

	PropVariantInit(&propvar);

	HRESULT hResult = framework->GetUICommandProperty(command, UI_PKEY_DecimalValue, &propvar);
	IFFAILEDRETURN(hResult);

	hResult = UIPropertyToDecimal(UI_PKEY_DecimalValue, propvar, &decimal);

	PropVariantClear(&propvar);

	return VarR4FromDec(&decimal, &value);
}

HRESULT RibbonControlManager::ComboBoxManager::ReplaceEntry(IUIFramework* framework, UINT command, std::basic_string<TCHAR> value, UINT category, BYTE position, BOOL invalidate)
{
	HRESULT hResult = RibbonControlManager::ComboBoxManager::SetEntry(framework, command, value, category, position, RibbonControlManager::ComboBoxManager::ACTION::REPLACE);

	if(SUCCEEDED(hResult))
	{
		if(invalidate)
		{
			hResult = RibbonControlManager::ComboBoxManager::Invalidate(framework, command);
		}
	}

	return hResult;
}

HRESULT RibbonControlManager::ComboBoxManager::RemoveEntry(IUIFramework* framework, UINT command, BYTE position)
{
	IUICollection* collection = NULL;

	HRESULT hResult = RibbonControlManager::ComboBoxManager::ChangePre(framework, command, &collection);

	if(SUCCEEDED(hResult))
	{
		hResult = collection->RemoveAt(position);
	}

	RibbonControlManager::ComboBoxManager::ChangePost(collection, NULL);

	return hResult;
}

HRESULT RibbonControlManager::ComboBoxManager::ClearContent(IUIFramework* framework, UINT command)
{
	IUICollection* collection = NULL;

	HRESULT hResult = RibbonControlManager::ComboBoxManager::ChangePre(framework, command, &collection);

	if(SUCCEEDED(hResult))
	{
		hResult = collection->Clear();

		RibbonControlManager::ComboBoxManager::Invalidate(framework, command);
	}

	RibbonControlManager::ComboBoxManager::ChangePost(collection, NULL);

	return hResult;
}

HRESULT RibbonControlManager::ComboBoxManager::SelectedEntry(IUIFramework* framework, UINT command, _OUT_(UINT & position))
{
	PROPVARIANT propvar;

	PropVariantInit(&propvar);

	HRESULT hResult = framework->GetUICommandProperty(command, UI_PKEY_SelectedItem, &propvar);
	IFFAILEDRETURN(hResult);

	hResult = UIPropertyToUInt32(UI_PKEY_SelectedItem, propvar, &position);

	PropVariantClear(&propvar);

	return hResult;
}

HRESULT RibbonControlManager::ComboBoxManager::SelectEntry(IUIFramework* framework, UINT command, USHORT position)
{
	UINT positionCurrent;
	PROPVARIANT propvar;

	if(framework == NULL)
	{
		return E_POINTER;
	}

	HRESULT hResult = RibbonControlManager::ComboBoxManager::SelectedEntry(framework, command, positionCurrent);
	IFFAILEDRETURN(hResult);

	if(position != positionCurrent)
	{
		PropVariantInit(&propvar);

		hResult = UIInitPropertyFromUInt32(UI_PKEY_SelectedItem, position, &propvar);
		IFFAILEDRETURN(hResult);

		hResult = framework->SetUICommandProperty(command, UI_PKEY_SelectedItem, propvar);

		PropVariantClear(&propvar);
	}

	return hResult;
}

HRESULT RibbonControlManager::ComboBoxManager::SetEntry(IUIFramework* framework, _IN_(UINT & command), _IN_(std::basic_string<TCHAR> & value), _IN_(UINT & category), SHORT position, RibbonControlManager::ComboBoxManager::ACTION action)
{
	RPropertySet* propertySet = NULL;
	IUICollection* collection = NULL;

	HRESULT hResult = RibbonControlManager::ComboBoxManager::ChangePre(framework, command, &collection);

	if(SUCCEEDED(hResult))
	{
		hResult = RPropertySet::CreateInstance(reinterpret_cast<IUISimplePropertySet**>(&propertySet));
		IFFAILEDRETURN(hResult);

		propertySet->InitializeItemProperties(NULL, value, category);

		switch(action)
		{
			case ComboBoxManager::ACTION::ADD:
				{
					if(position < 0)
					{
						hResult = collection->Add(propertySet);
					}
					else
					{
						hResult = collection->Insert(position, propertySet);
					}
				}
				break;

			case ComboBoxManager::ACTION::REPLACE:
				{
					hResult = collection->Replace(position, propertySet);
				}
				break;
		}
	}

	RibbonControlManager::ComboBoxManager::ChangePost(collection, propertySet);

	return hResult;
}

HRESULT RibbonControlManager::ComboBoxManager::ChangePre(IUIFramework* framework, _IN_(UINT & command), _OUT_(IUICollection** collection))
{
	PROPVARIANT propvar;

	if(framework == NULL)
	{
		return E_POINTER;
	}

	PropVariantInit(&propvar);

	HRESULT hResult = framework->GetUICommandProperty(command, UI_PKEY_ItemsSource, &propvar);
	IFFAILEDRETURN(hResult);

	hResult = UIPropertyToInterface(UI_PKEY_ItemsSource, propvar, collection);

	PropVariantClear(&propvar);

	return hResult;
}

VOID RibbonControlManager::ComboBoxManager::ChangePost(IUICollection* collection, RPropertySet* propertySet)
{
	if(collection != NULL)
	{
		collection->Release();
	}

	if(propertySet != NULL)
	{
		propertySet->Release();
	}
}