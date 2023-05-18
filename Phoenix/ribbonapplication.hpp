#pragma once
//-----
#include <map>
#include "exceptionhandler.hpp"
#include "ribbonxml.h" //through uicc.exe generated header from xml
#include "rbuttonhandler.hpp"
#include "rcheckboxhandler.hpp"
#include "rcomboboxhandler.hpp"
#include "rdropdownbuttonhandler.hpp"
#include "rdropdowngalleryhandler.hpp"
#include "rspinnerhandler.hpp"
#include "rtogglebuttonhandler.hpp"
#include <string>
#include <iterator>
//----
enum mCONTROLGROUP {PROJECT, OBJECT, CSGNODE};
//---
//--
class UnionValue //or boost::variant or std::variant (C++17)
{
	public:
		enum class DATATYPE : BYTE {TINT, TSTRING};

		UnionValue(INT intValue);
		UnionValue(std::basic_string<TCHAR> stringValue);
		UnionValue(_IN_(UnionValue & in));
		~UnionValue();

		operator INT();
		operator std::basic_string<TCHAR>();

	private:
		DATATYPE dataType;

		union
		{
			INT intValue;
			std::basic_string<TCHAR> stringValue;
		};
};

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

		std::array<BOOL, 3> controlsEnabled;

		std::map<std::basic_string<TCHAR>, std::vector<UnionValue>> mutableControls;

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

		BOOL GetLocation(_IN_(std::basic_string<TCHAR> & control), std::map<std::basic_string<TCHAR>, std::vector<UnionValue>>::iterator & location)
		{
			location = this->mutableControls.find(control);

			if(location != this->mutableControls.end())
			{
				if(!location->second.empty())
				{
					return TRUE;
				}
			}

			return FALSE;
		}

		VOID EraseMutableControl(std::map<std::basic_string<TCHAR>, std::vector<UnionValue>>::iterator & location, BOOL erase)
		{
			if(erase)
			{
				if(location != this->mutableControls.end())
				{
					this->mutableControls.erase(location);
				}
			}
		}

		template <class T>
		VOID GetValue(_IN_(std::basic_string<TCHAR> & control), _OUT_(T & value), BOOL erase = TRUE)
		{
			std::map<std::basic_string<TCHAR>, std::vector<UnionValue>>::iterator location;

			if(this->GetLocation(control, location))
			{
				value = static_cast<T>(location->second.front());
			}

			this->EraseMutableControl(location, erase);
		}

		template <class T>
		VOID GetValues(_IN_(std::basic_string<TCHAR> & control), _OUT_(std::vector<T> & values), BOOL erase = TRUE)
		{
			std::map<std::basic_string<TCHAR>, std::vector<UnionValue>>::iterator location;

			if(this->GetLocation(control, location))
			{
				values.clear();

				std::copy(location->second.begin(), location->second.end(), std::back_inserter(values));
			}

			this->EraseMutableControl(location, erase);
		}

		LONG cRef;
		HWND hWnd;
};