#pragma once
//-----
#include "ribbonfunctions.hpp"
//----
//---
//--

//main abstract control class, it inherits from IUICommandHandler and implements its pure virtual methods AddRef, Release, QueryInterface, UpdateProperty and Execute
class RControlHandler : public IUICommandHandler
{
	public:
		RControlHandler(HWND hWnd, _IN_(BOOL & enabled));
		virtual ~RControlHandler() = 0;

		//purpose - enables to use only one instance of the control
		//body - the lock variable is incremented
		//in:
		//	X
		//out:
		//	
		inline STDMETHODIMP_(ULONG) AddRef()
		{
			return InterlockedIncrement(&this->cRef);
		}

		//purpose - enables to use only one instance of the control
		//body - the lock variable is decremented
		//in:
		//	X
		//out:
		//	
		STDMETHODIMP_(ULONG) Release();

		//purpose - enables to use only one instance of the control
		//body - a pointer to the control itself is returned, the method AddRef is called
		//in:
		//	param(1) - object reference id
		//	param(2) - pointer, which will point to the control
		//out:
		//	
		STDMETHODIMP QueryInterface(_IN_(IID & iid), _OUT_(VOID** ppv));

		//purpose - implementation of the pure virtual method of the class IUICommandHandler
		//body -
		//in:
		//	params(...)
		//out:
		//	
		STDMETHOD(UpdateProperty)(UINT nCmdID, _IN_(PROPERTYKEY & key), _IN_(PROPVARIANT* pPropvarCurrentValue), _OUT_(PROPVARIANT* pPropvarNewValue));

		//purpose - implementation of the pure virtual method of the class IUICommandHandler
		//body -
		//in:
		//	params(...)
		//out:
		//	
		STDMETHOD(Execute)(UINT nCmdID, UI_EXECUTIONVERB verb, _IN_(PROPERTYKEY* key), _IN_(PROPVARIANT* pPropvarValue), IUISimplePropertySet* pCommandExecutionProperties);

	protected:
		//lock variable, when a control is created, then the value is set to 1, the value is incremented (AddRef, QueryInterface) and decremented (Release) by using the control,
		//that means, that there will be used only one instance of the control, if the value reach 0, the control can be deleted
		LONG cRef;
		//handle to the window to which the messages are sent
		HWND hWnd;
		//control is enabled or disabled on create
		BOOL enabled;
};