#include "rgallerycontrolhandler.hpp"

RGalleryControlHandler::RGalleryControlHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(std::vector<Property> & content)):
RControlHandler(hWnd, enabled),
content(content)
{
}

RGalleryControlHandler::~RGalleryControlHandler()
{
}

STDMETHODIMP RGalleryControlHandler::UpdateProperty(UINT nCmdID, _IN_(PROPERTYKEY & key), _IN_(PROPVARIANT* pPropvarCurrentValue), _OUT_(PROPVARIANT* pPropvarNewValue))
{
	HRESULT hResult = RControlHandler::UpdateProperty(nCmdID, key, pPropvarCurrentValue, pPropvarNewValue);

	UNREFERENCED_PARAMETER(nCmdID);
	UNREFERENCED_PARAMETER(pPropvarNewValue);

	if(hResult == E_NOTIMPL)
	{
		if(key == UI_PKEY_Categories)
		{
			hResult = RGalleryControlHandler::SetContent(pPropvarCurrentValue, Property::CONTENTTYPE::CATEGORY);
		}
		else if(key == UI_PKEY_ItemsSource)
		{
			hResult = RGalleryControlHandler::SetContent(pPropvarCurrentValue, Property::CONTENTTYPE::COMMAND | Property::CONTENTTYPE::ITEM);

			if(SUCCEEDED(hResult))
			{
				this->content.clear();
			}
		}
	}

	return hResult;
}

HRESULT RGalleryControlHandler::SetContent(_IN_(PROPVARIANT* pPropvarValue), BYTE type)
{
	RPropertySet* propertySet = NULL;
	IUICollection* collection = NULL;

	HRESULT hResult = UIPropertyToInterface(UI_PKEY_ItemsSource, *pPropvarValue, &collection);
	IFFAILEDRETURN(hResult);

	for(std::vector<Property>::const_iterator i = this->content.begin(), j = this->content.end(); i != j; i++)
	{
		if(i->type & type)
		{
			hResult = RPropertySet::CreateInstance(reinterpret_cast<IUISimplePropertySet**>(&propertySet));
			IFFAILEDRETURN(hResult);

			switch(i->type)
			{
				case Property::CONTENTTYPE::CATEGORY:
					{
						propertySet->InitializeCategoryProperties(i->label, i->categoryId);
					}
					break;

				case Property::CONTENTTYPE::COMMAND:
					{
						propertySet->InitializeCommandProperties(i->categoryId, i->commandId, i->commandType);
					}
					break;

				case Property::CONTENTTYPE::ITEM:
					{
						propertySet->InitializeItemProperties(NULL, i->label, i->categoryId);
					}
					break;
			}

			hResult = collection->Add(propertySet);
			IFFAILEDRETURN(hResult);

			propertySet->Release();
		}
	}

	collection->Release();

	return hResult;
}