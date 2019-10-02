#pragma once
//-----
#include <vector>
#include <windows.h>
#include <shobjidl.h>
#include <tchar.h>
#include "resource.h"
#include "macros.hpp"
//----
//---
//--
class Dialog
{
	public:
		static HRESULT open(_IN_(std::basic_string<TCHAR> & title), _IN_(std::vector<COMDLG_FILTERSPEC> & types), _OUT_(std::basic_string<TCHAR> & file));
		static HRESULT save(_IN_(std::basic_string<TCHAR> & title), _IN_(std::vector<COMDLG_FILTERSPEC> & types), _OUT_(std::basic_string<TCHAR> & file));

		class MessageDialog
		{
			public:
				enum class TYPE : unsigned char {M_INFO, M_WARNING, M_ERROR, M_QUESTION};

				typedef struct dialogValues
				{
					Dialog::MessageDialog::TYPE type;

					std::basic_string<TCHAR> title;
					std::basic_string<TCHAR> text;
				}DIALOGVALUES;

				static BOOL show(HWND hWnd, Dialog::MessageDialog::TYPE type, std::basic_string<TCHAR> title, std::basic_string<TCHAR> text);

			private:
				static INT_PTR CALLBACK messageDialogProcedure(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
		};

		static VOID setControlBitmap(HWND hWnd, UINT control, USHORT resourceBitmap);
		static VOID deleteControlBitmap(HWND hWnd, UINT control);

		static HFONT setControlFont(HWND hWnd, UINT control, BYTE height, USHORT weight);
		static VOID deleteControlFont(HWND hWnd, UINT control);

	private:
		static VOID setTitle(_INOUT_(std::basic_string<TCHAR> & title), std::basic_string<TCHAR> append);

		static HRESULT dialog(CLSID key, _IN_(std::basic_string<TCHAR> & title), _IN_(std::vector<COMDLG_FILTERSPEC> & types), _OUT_(std::basic_string<TCHAR> & file));
};