#pragma once
//-----
//WIN32_LEAN_AND_MEAN is defined in Visual Studio project
//#ifndef WIN32_LEAN_AND_MEAN
//	#define WIN32_LEAN_AND_MEAN
//#endif
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <windows.h>
#include <windowsx.h>
#include <strsafe.h>
#include <shellapi.h>
#include <shlobj.h>
#include "macros.hpp"
#include "regularexpression.hpp"
#include "windowsui.hpp"
#include "ribbon.hpp"
#include "ribboncontrolgroups.hpp"
#include "ribboncontrolmanager.hpp"
#include "dialog.hpp"
#include "timer.hpp"
#include "printscreen.hpp"
#include "mouse.hpp"
#include "input.hpp"
#include "project.hpp"
#include "objectc.hpp"
#include "carve.hpp"

//for syslink
#ifdef UNICODE
	#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
//----
#define FORMATNUM(X) std::setfill(_T('0')) << std::setw(4) << X
//---
typedef struct transformationValues
{
	transformationValues():
	set(FALSE),
	axis(mCOORDINATE::X),
	axisRibbon(mCOORDINATE::X),
	type(Transformation::TYPE::TRANSLATE),
	step(1.0f)
	{
	}

	BOOL set;
	mCOORDINATE axis;
	mCOORDINATE axisRibbon;
	Transformation::TYPE type;
	FLOAT step;
}TRANSFORMATIONVALUES;

typedef struct infoMessage
{
	infoMessage():
	position(std::array<USHORT, 2>{0, 0})
	{
	}

	std::array<USHORT, 2> position;
	std::basic_string<TCHAR> message;
}INFOMESSAGE;
//--
VOID createUI(HINSTANCE hInstance);
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);
VOID writeInfoMessage(HWND hWnd, IUIFramework* framework, _IN_(UINT & command), std::basic_string<TCHAR> reset, std::basic_string<TCHAR> message);
BOOL handleFloatListControl(HWND hWnd, IUIFramework* framework, _IN_(UINT & command), _IN_(std::vector<UINT> & commands), _OUT_(std::basic_string<TCHAR> & string), _OUT_(UINT & type), std::basic_string<TCHAR> reset, std::basic_string<TCHAR> message);
VOID formatLightMaterialString(_IN_(ARRAY4REF(FLOAT, values)), _OUT_(std::basic_string<TCHAR> & string), BYTE lightMaterialType);
BOOL getExePath(_OUT_(std::basic_string<TCHAR> & path));
VOID free();

INT_PTR CALLBACK helpDialogProcedure(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

inline VOID openUrl(std::basic_string<TCHAR> url)
{
	ShellExecute(NULL, _T("open"), url.data(), NULL, NULL, SW_SHOWNORMAL);
}