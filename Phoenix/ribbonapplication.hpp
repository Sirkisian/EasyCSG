#pragma once
//-----
#include "exceptionhandler.hpp"
#include "ribbonxml.h" //through uicc.exe generated header from xml
#include "rbuttonhandler.hpp"
#include "rcheckboxhandler.hpp"
#include "rcomboboxhandler.hpp"
#include "rdropdownbuttonhandler.hpp"
#include "rdropdowngalleryhandler.hpp"
#include "rspinnerhandler.hpp"
#include "rtogglebuttonhandler.hpp"
//----
enum mCONTROLGROUP {PROJECT, OBJECT};
//---
//--
class RibbonApplication : public IUIApplication
{
	public:
		static inline HRESULT CreateInstance(_OUT_(IUIApplication** ppApplication), HWND hWnd)
		{
			return RibbonFunctions::CreateInstance(reinterpret_cast<IUnknown**>(ppApplication), new RibbonApplication(hWnd));
		}

		virtual ~RibbonApplication();

		inline STDMETHOD_(ULONG, AddRef)()
		{
			return InterlockedIncrement(&this->cRef);
		}

		STDMETHOD_(ULONG, Release)();
		STDMETHOD(QueryInterface)(_IN_(IID & iid), _OUT_(VOID** ppv));

		STDMETHOD(OnViewChanged)(UINT32 nViewID, UI_VIEWTYPE typeID, IUnknown* pView, UI_VIEWVERB verb, INT32 uReasonCode);
		STDMETHOD(OnCreateUICommand)(UINT32 nCmdID, UI_COMMANDTYPE typeID, IUICommandHandler** ppCommandHandler);
		STDMETHOD(OnDestroyUICommand)(UINT32 commandId, UI_COMMANDTYPE typeID, IUICommandHandler* pCommandHandler);

		std::array<BOOL, 2> controlsEnabled;

	private:
		static HRESULT AddButton(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, BOOL enabled);
		static HRESULT AddCheckBox(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, BOOL enabled, BOOL defValue);
		static HRESULT AddComboBox(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, BOOL enabled, _IN_(std::vector<Property> & propertySet), USHORT selected = 0);
		static HRESULT AddDropDownButton(_OUT_(IUICommandHandler** ppCommandHandler), BOOL enabled);
		static HRESULT AddDropDownGallery(_OUT_(IUICommandHandler** ppCommandHandler), BOOL enabled, _IN_(std::vector<Property> & propertySet));
		static HRESULT AddSpinner(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, BOOL enabled, FLOAT minValue, FLOAT maxValue, FLOAT increment);
		static HRESULT AddToggleButton(_OUT_(IUICommandHandler** ppCommandHandler), HWND hWnd, BOOL enabled, BOOL defValue);
		static HRESULT AddControl(_OUT_(IUICommandHandler** ppCommandHandler), HRESULT createResult, _OUT_(IUICommandHandler** control));

		RibbonApplication(HWND hWnd);

		LONG cRef;
		HWND hWnd;
};