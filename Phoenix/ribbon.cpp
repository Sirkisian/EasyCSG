#include "ribbon.hpp"

Ribbon::Ribbon():
framework(NULL),
application(NULL)
{
}

Ribbon::~Ribbon()
{
}

//the function CoInitializeEx need to be called before
HRESULT Ribbon::CreateRibbon(HWND hWnd, LPCTSTR rcName)
{
	HRESULT hResult = CoCreateInstance(CLSID_UIRibbonFramework, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&this->framework));
	IFFAILEDRETURN(hResult);

	hResult = RibbonApplication::CreateInstance(reinterpret_cast<IUIApplication**>(&this->application), hWnd);
	IFFAILEDRETURN(hResult);

	hResult = this->framework->Initialize(hWnd, this->application);
	IFFAILEDRETURN(hResult);

	//through uicc.exe given name /name:xyz, can be found in .rc file, the .rc file need to be add to the project 
	hResult = this->framework->LoadUI(GetModuleHandle(NULL), rcName);

	this->application->Release();

	return hResult;
}

HRESULT Ribbon::DestroyRibbon()
{
	HRESULT hResult = E_FAIL;

	if(this->framework != NULL)
	{
		hResult = this->framework->Destroy(); //the method delete the IUIApplication object from InitializeFramework
		this->framework->Release(); 
		this->framework = NULL;

		this->application = NULL;
	}

	return hResult;
}

HRESULT Ribbon::GetRibbonHeight(_OUT_(UINT* ribbonHeight))
{
	HRESULT hResult = E_FAIL;
	IUIRibbon* ribbon = NULL;

	if(this->framework != NULL)
	{
		hResult = this->framework->GetView(0, IID_PPV_ARGS(&ribbon));
		IFFAILEDRETURN(hResult);

		hResult = ribbon->GetHeight(ribbonHeight);

		ribbon->Release();
	}

	return hResult;
}

VOID Ribbon::EnableControlsOnCreate(BOOL value, mCONTROLGROUP group)
{
	if(this->application != NULL)
	{
		this->application->controlsEnabled[group] = value;
	}
}

VOID Ribbon::SetMutableControl(std::basic_string<TCHAR> control, std::vector<UnionValue> value)
{
	std::map<std::basic_string<TCHAR>, std::vector<UnionValue>>::iterator location = this->application->mutableControls.find(control);

	if(location != this->application->mutableControls.end())
	{
		location->second.clear();

		std::copy(value.begin(), value.end(), std::back_inserter(location->second));
	}
}

HRESULT Ribbon::SetColor(_IN_(PROPERTYKEY & key), _IN_(BYTE & hue), _IN_(BYTE & saturation), _IN_(BYTE & brightness))
{
	PROPVARIANT propvarColor;
	IPropertyStore* propertyStore = NULL;
	UI_HSBCOLOR hsbColor = UI_HSB(hue, saturation, brightness);

	PropVariantInit(&propvarColor);

	HRESULT hResult = this->framework->QueryInterface(IID_PPV_ARGS(&propertyStore));
	IFFAILEDRETURN(hResult);

	hResult = InitPropVariantFromUInt32(hsbColor, &propvarColor);
	IFFAILEDRETURN(hResult);

	hResult = propertyStore->SetValue(key, propvarColor);
	IFFAILEDRETURN(hResult);

	hResult = propertyStore->Commit();

	propertyStore->Release();

	PropVariantClear(&propvarColor);

	return hResult;
}