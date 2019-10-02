#pragma once
//-----
#include <vector>
#include "rpropertyset.hpp"
#include "rcontrolhandler.hpp"
//----
//---
//--

//abstract control class for gallery controls as Combo Box and Drop-Down Gallery
class RGalleryControlHandler : public RControlHandler
{
	public:
		RGalleryControlHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(std::vector<Property> & content));
		virtual ~RGalleryControlHandler() = 0;

		STDMETHOD(UpdateProperty)(UINT nCmdID, _IN_(PROPERTYKEY & key), _IN_(PROPVARIANT* pPropvarCurrentValue), _OUT_(PROPVARIANT* pPropvarNewValue));

	protected:
		//purpose -enables to set the content of a gallery control
		//body - category, command or item is added to the collection of the control
		//in:
		//	param(1) - collection of the gallery control, to witch the content is added
		//	param(2) - bitmask of the content type
		//out:
		//	
		HRESULT SetContent(_IN_(PROPVARIANT* pPropvarValue), BYTE type);

		//content of the gallery control used on create
		std::vector<Property> content;
};