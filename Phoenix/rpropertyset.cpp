#include "rpropertyset.hpp"

Property::Property(std::basic_string<TCHAR> label, UINT categoryId, UINT commandId, UI_COMMANDTYPE commandType, Property::CONTENTTYPE type):
categoryId(categoryId),
commandId(commandId),
commandType(commandType),
type(type)
{
	this->label = label.substr(0, Property::labelSize);
}

Property::~Property()
{
}

RPropertySet::RPropertySet():
cRef(1),
categoryId(UI_COLLECTION_INVALIDINDEX),
commandId(0),
commandType(UI_COMMANDTYPE_UNKNOWN),
image(NULL)
{
}

RPropertySet::~RPropertySet()
{
	if(this->image != NULL)
	{
		this->image->Release();
	}
}

STDMETHODIMP_(ULONG) RPropertySet::Release()
{
	LONG cRef = InterlockedDecrement(&this->cRef);

	if(cRef == 0)
	{
		delete this;
	}

	return cRef;
}

STDMETHODIMP RPropertySet::QueryInterface(_IN_(IID & iid), _OUT_(VOID** ppv))
{
	HRESULT hResult = RibbonFunctions::QueryInterface(iid, ppv, *this);

	if(SUCCEEDED(hResult))
	{
		this->AddRef();
	}

	return hResult;
}

STDMETHODIMP RPropertySet::GetValue(_IN_(PROPERTYKEY & key), _OUT_(PROPVARIANT* pPropvar))
{
	HRESULT hResult = E_FAIL;

	if(key == UI_PKEY_ItemImage)
	{
		if(this->image != NULL)
		{
			hResult = UIInitPropertyFromImage(key, this->image, pPropvar);
		}
		else
		{
			hResult = S_FALSE;
		}
	}
	else if(key == UI_PKEY_Label)
	{
		hResult = UIInitPropertyFromString(key, this->label.data(), pPropvar);
	}
	else if(key == UI_PKEY_CategoryId)
	{
		hResult = UIInitPropertyFromUInt32(key, this->categoryId, pPropvar);
	}
	else if(key == UI_PKEY_CommandId)
	{
		if(this->commandId > 0)
		{
			hResult = UIInitPropertyFromUInt32(key, this->commandId, pPropvar);
		}
		else 
		{
			hResult = S_FALSE;
		}
	}
	else if(key == UI_PKEY_CommandType)
	{
		hResult = UIInitPropertyFromUInt32(key, this->commandType, pPropvar);
	}

	return hResult;
}

VOID RPropertySet::InitializeCommandProperties(_IN_(UINT & categoryId), _IN_(UINT & commandId), _IN_(UI_COMMANDTYPE & commandType))
{
	this->categoryId = categoryId;
	this->commandId = commandId;
	this->commandType = commandType;
}

VOID RPropertySet::InitializeItemProperties(IUIImage* image, _IN_(std::basic_string<TCHAR> & label), _IN_(UINT & categoryId))
{
	this->image = image;

	if(this->image != NULL)
	{
		this->image->AddRef();
	}

	this->label = label.substr(0, Property::labelSize);

	this->categoryId = categoryId;
}

VOID RPropertySet::InitializeCategoryProperties(_IN_(std::basic_string<TCHAR> & label), _IN_(UINT & categoryId))
{
	this->label = label.substr(0, Property::labelSize);

	this->categoryId = categoryId;
}