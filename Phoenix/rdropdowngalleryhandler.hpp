#pragma once
//-----
#include "rgallerycontrolhandler.hpp"
//----
//---
//--

//Drop-Down Gallery
class RDropDownGalleryHandler : public RGalleryControlHandler
{
	public:
		static inline HRESULT CreateInstance(_OUT_(IUICommandHandler** ppCommandHandler), _IN_(BOOL & enabled), _IN_(std::vector<Property> & galleryContent))
		{
			return RibbonFunctions::CreateInstance(reinterpret_cast<IUnknown**>(ppCommandHandler), new RDropDownGalleryHandler(enabled, galleryContent));
		}

		virtual ~RDropDownGalleryHandler();

	private:
		RDropDownGalleryHandler(_IN_(BOOL & enabled), _IN_(std::vector<Property> & galleryContent));
};