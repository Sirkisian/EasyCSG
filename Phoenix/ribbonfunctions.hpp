#pragma once
//-----
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <uiribbon.h>
#include <uiribbonpropertyhelpers.h>
#include <string>
#include "macros.hpp"
//----
//value, which is sent from Ribbon controls to the WindowProcedure as wParam
#define WM_RIBBON 0xAFAF

//macro used to handle HRESULT return values from functions/methods
#define IFFAILEDRETURN(X) if(FAILED(X)){return X;}
//---
//--
namespace RibbonFunctions
{
	//purpose - help function used by Ribbon controls in their CreateInstance method
	//body - assign param(2) to param(1)
	//in:
	//	param(1) - pointer, which will point to the new allocated object param(2)
	//	param(2) - pointer to the new allocated object
	//out:
	//	
	HRESULT CreateInstance(_OUT_(IUnknown** ppObject), IUnknown* pObject);

	//purpose - help function used by Ribbon controls in their QueryInterface method
	//body - assign param(3) to param(2)
	//in:
	//	param(1) - object reference id
	//	param(2) - pointer, which will point to the object param(3)
	//	param(3) - pointer to the object itself
	//out:
	//	
	HRESULT QueryInterface(_IN_(IID & iid), _OUT_(VOID** ppv), IUnknown & pObject);
}