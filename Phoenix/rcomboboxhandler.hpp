#pragma once
//-----
#include "rgallerycontrolhandler.hpp"
//----
#define STRINGVALUE -1
//---
//--

//Combo Box
class RComboBoxHandler : public RGalleryControlHandler
{
	public:
		static inline HRESULT CreateInstance(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, _IN_(BOOL & enabled), _IN_(std::vector<Property> & comboboxContent), _IN_(USHORT & selected))
		{
			return RibbonFunctions::CreateInstance(reinterpret_cast<IUnknown**>(ppCommandHandler), new RComboBoxHandler(hWnd, enabled, comboboxContent, selected));
		}

		virtual ~RComboBoxHandler();

		STDMETHOD(UpdateProperty)(UINT nCmdID, _IN_(PROPERTYKEY & key), _IN_(PROPVARIANT* pPropvarCurrentValue), _OUT_(PROPVARIANT* pPropvarNewValue));
		STDMETHOD(Execute)(UINT nCmdID, UI_EXECUTIONVERB verb, _IN_(PROPERTYKEY* key), _IN_(PROPVARIANT* pPropvarValue), IUISimplePropertySet* pCommandExecutionProperties);

	private:
		RComboBoxHandler(HWND hWnd, _IN_(BOOL & enabled), _IN_(std::vector<Property> & comboboxContent), _IN_(USHORT & selected));

		//selected entry on create
		USHORT selected;
};