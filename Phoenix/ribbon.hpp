#pragma once
//-----
#include <combaseapi.h>
#include "ribbonapplication.hpp"
//----
//---
//--

//the Ribbon implementation is based on the Microsoft "Gallery" sample 
class Ribbon
{
	public:
		Ribbon();
		~Ribbon();

		HRESULT CreateRibbon(HWND hWnd, LPCTSTR rcName);
		HRESULT DestroyRibbon();

		HRESULT GetRibbonHeight(_OUT_(UINT* ribbonHeight));

		inline HRESULT SetBackgroundColor(BYTE hue, BYTE saturation, BYTE brightness)
		{
			return this->SetColor(UI_PKEY_GlobalBackgroundColor, hue, saturation, brightness);
		}

		inline HRESULT SetTextColor(BYTE hue, BYTE saturation, BYTE brightness)
		{
			return this->SetColor(UI_PKEY_GlobalTextColor, hue, saturation, brightness);
		}

		VOID EnableControlsOnCreate(BOOL value, mCONTROLGROUP group);

		inline IUIFramework* GetFramework()
		{
			return this->framework;
		}

		VOID SetMutableControl(std::basic_string<TCHAR> control, std::vector<UnionValue> value);

	private:
		HRESULT SetColor(_IN_(PROPERTYKEY & key), _IN_(BYTE & hue), _IN_(BYTE & saturation), _IN_(BYTE & brightness));

		IUIFramework* framework;
		RibbonApplication* application;
};