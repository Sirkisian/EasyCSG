#pragma once
//-----
#include <vector>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <windows.h>
#include <tchar.h>
#include <commctrl.h>
#include "resource.h"
#include "macros.hpp"
#include "exceptionhandler.hpp"
#include <string>
//----
enum mWINDOW {MAIN = 1, STATUSBAR, CHILD};
//---
//--
class ClassProperty
{
	public:
		static const BYTE nameSize = 25;

		ClassProperty(_IN_(std::basic_string<TCHAR> & name), UINT style, HBRUSH backgroundColor, WNDPROC windowProcedure);
		~ClassProperty();

		std::basic_string<TCHAR> name;
		UINT style;
		HBRUSH backgroundColor;
		WNDPROC windowProcedure;
};

class WindowProperty
{
	public:
		static const BYTE nameSize = 35;

		WindowProperty(_IN_(std::basic_string<TCHAR> & name), DWORD style, _IN_(ARRAY2REF(SHORT, position)), _IN_(ARRAY2REF(SHORT, size)), USHORT id, USHORT parentId, BOOL show);
		~WindowProperty();

		std::basic_string<TCHAR> name;
		DWORD style;
		std::array<SHORT, 2> position;
		std::array<SHORT, 2> size;
		USHORT id;
		USHORT parentId;
		BOOL show;
};

class StatusBarProperty
{
	public:
		static const BYTE contentSize = 255;

		StatusBarProperty(_IN_(std::basic_string<TCHAR> & content), USHORT id, USHORT parentId, COLORREF backgroundColor);
		~StatusBarProperty();

		std::basic_string<TCHAR> content;
		USHORT id;
		USHORT parentId;
		COLORREF backgroundColor;
};

class Window
{
	public:
		Window(HWND hWnd, USHORT id, HDC hDC = NULL, HGLRC hRC = NULL);
		~Window();

		HWND hWnd;
		USHORT id;
		HDC hDC;
		HGLRC hRC;
};

class ComparatorWindow
{
	public:
		ComparatorWindow(USHORT id):
		id(id)
		{
		}

		BOOL operator()(_IN_(Window* window))
		{
			return window->id == this->id;
		}

	private:
		USHORT id;
};

class WindowsUI
{
	public:
		static VOID getScreenSize(_OUT_(ARRAY2REF(SHORT, size)));

		WindowsUI();
		~WindowsUI();

		VOID createUI(HINSTANCE hInstance, _IN_(ClassProperty & cp), _IN_(WindowProperty & wp), _IN_(StatusBarProperty & sp));

		HWND getHwnd(USHORT id);

		BOOL createGraphicWindow(_IN_(WindowProperty & wp));
		BOOL moveBack2Front(USHORT id);

		VOID registerHotkey(HWND hWnd, INT id, UINT systemKey, UINT virtualKey);

		VOID destroyWindow(USHORT id);

		VOID release();

	private:
		static ATOM registerClass(HINSTANCE hInstance, _IN_(ClassProperty & cp));

		static VOID releaseContext(Window* window);

		static BOOL setPixelFormat(HDC hDC);

		static BOOL setMsAA(Window* window);

		BOOL createWindow(HINSTANCE hInstance, _IN_(std::basic_string<TCHAR> & className), _IN_(WindowProperty & wp));

		VOID getWindowMessages();

		Window* getWindowById(_IN_(USHORT & id));

		HINSTANCE hInstance;

		std::vector<INT> hotkey;
		std::vector<Window*> windows;
};