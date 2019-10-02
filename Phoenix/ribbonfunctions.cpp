#include "ribbonfunctions.hpp"

HRESULT RibbonFunctions::CreateInstance(_OUT_(IUnknown** ppObject), IUnknown* pObject)
{
	if(ppObject == NULL)
	{
		return E_POINTER;
	}

	if(pObject != NULL)
	{
		*ppObject = pObject;
	}
	else
	{
		return E_OUTOFMEMORY;
	}

	return S_OK;
}

HRESULT RibbonFunctions::QueryInterface(_IN_(IID & iid), _OUT_(VOID** ppv), IUnknown & pObject)
{
	if(ppv == NULL)
	{
		return E_POINTER;
	}

	if(iid == __uuidof(IUnknown))
	{
		*ppv = static_cast<IUnknown*>(&pObject);
	}
	else if(iid == __uuidof(IUIApplication))
	{
		*ppv = static_cast<IUIApplication*>(&pObject);
	}
	else if(iid == __uuidof(IUICommandHandler))
	{
		*ppv = static_cast<IUICommandHandler*>(&pObject);
	}
	else if(iid == __uuidof(IUISimplePropertySet))
	{
		*ppv = static_cast<IUISimplePropertySet*>(&pObject);
	}
	else
	{
		*ppv = NULL;
		
		return E_NOINTERFACE;
	}

	return S_OK;
}