#pragma once
//-----
#include "ribbonfunctions.hpp"
//----
//---
//--
class Property
{
	public:
		static const BYTE labelSize = 50;
		enum CONTENTTYPE {CATEGORY = 1, COMMAND, ITEM = 4};

		Property(std::basic_string<TCHAR> label, UINT categoryId, UINT commandId, UI_COMMANDTYPE commandType, Property::CONTENTTYPE type);
		~Property();

		std::basic_string<TCHAR> label;
		UINT categoryId;
		UINT commandId;
		UI_COMMANDTYPE commandType;
		CONTENTTYPE type;
};

class RPropertySet : public IUISimplePropertySet
{
	public:
		static inline HRESULT CreateInstance(_OUT_(IUISimplePropertySet** ppSimplePropertySet))
		{
			return RibbonFunctions::CreateInstance(reinterpret_cast<IUnknown**>(ppSimplePropertySet), new RPropertySet());
		}

		virtual ~RPropertySet();

		STDMETHOD(GetValue)(_IN_(PROPERTYKEY & key), _OUT_(PROPVARIANT* pPropvar));

		inline STDMETHOD_(ULONG, AddRef)()
		{
			return InterlockedIncrement(&this->cRef);
		}

		STDMETHOD_(ULONG, Release)();
		STDMETHOD(QueryInterface)(_IN_(IID & iid), _OUT_(VOID** ppv));

		VOID InitializeCommandProperties(_IN_(UINT & categoryId), _IN_(UINT & commandId), _IN_(UI_COMMANDTYPE & commandType));
		VOID InitializeItemProperties(IUIImage* image, _IN_(std::basic_string<TCHAR> & label), _IN_(UINT & categoryId));
		VOID InitializeCategoryProperties(_IN_(std::basic_string<TCHAR> & label), _IN_(UINT & categoryId));

	private:
		RPropertySet();

		LONG cRef;
		std::basic_string<TCHAR> label;
		UINT categoryId;
		UINT commandId;
		UI_COMMANDTYPE commandType;
		IUIImage* image;
};