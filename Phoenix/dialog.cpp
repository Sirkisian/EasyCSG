#include "dialog.hpp"

HRESULT Dialog::open(_IN_(std::basic_string<TCHAR> & title), _IN_(std::vector<COMDLG_FILTERSPEC> & types), _OUT_(std::basic_string<TCHAR> & file))
{
	std::basic_string<TCHAR> dialogTitle{title};

	Dialog::setTitle(dialogTitle, std::basic_string<TCHAR>{_T("Open")});

	return Dialog::dialog(CLSID_FileOpenDialog, dialogTitle, types, file);
}

HRESULT Dialog::save(_IN_(std::basic_string<TCHAR> & title), _IN_(std::vector<COMDLG_FILTERSPEC> & types), _OUT_(std::basic_string<TCHAR> & file))
{
	std::basic_string<TCHAR> dialogTitle{title};

	Dialog::setTitle(dialogTitle, std::basic_string<TCHAR>{_T("Save")});

	return Dialog::dialog(CLSID_FileSaveDialog, dialogTitle, types, file);
}

BOOL Dialog::MessageDialog::show(HWND hWnd, Dialog::MessageDialog::TYPE type, std::basic_string<TCHAR> title, std::basic_string<TCHAR> text)
{
	Dialog::MessageDialog::DIALOGVALUES values;

	values.type = type;
	values.title = title;
	values.text = text;

	INT_PTR returnValue = DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MESSAGEBOX), hWnd, Dialog::MessageDialog::messageDialogProcedure, reinterpret_cast<LPARAM>(&values));

	return returnValue == IDYES || returnValue == IDOK;
}

INT_PTR CALLBACK Dialog::MessageDialog::messageDialogProcedure(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			{
				Dialog::MessageDialog::DIALOGVALUES* values = reinterpret_cast<Dialog::MessageDialog::DIALOGVALUES*>(lParam);

				if(values != NULL)
				{
					Dialog::setControlFont(hWndDlg, IDC_STATIC_M1, 18, FW_BOLD);
					SendDlgItemMessage(hWndDlg, IDC_STATIC_M1, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(values->title.data()));

					Dialog::setControlFont(hWndDlg, IDC_STATIC_M3, 15, FW_MEDIUM);
					SendDlgItemMessage(hWndDlg, IDC_STATIC_M3, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(values->text.data()));

					switch(values->type)
					{
						case Dialog::MessageDialog::TYPE::M_INFO:
							{
								Dialog::setControlBitmap(hWndDlg, IDC_STATIC_M2, IDB_M_INFO);
							}
							break;

						case Dialog::MessageDialog::TYPE::M_WARNING:
							{
								Dialog::setControlBitmap(hWndDlg, IDC_STATIC_M2, IDB_M_WARNING);
							}
							break;

						case Dialog::MessageDialog::TYPE::M_ERROR:
							{
								Dialog::setControlBitmap(hWndDlg, IDC_STATIC_M2, IDB_M_ERROR);
							}
							break;

						case Dialog::MessageDialog::TYPE::M_QUESTION:
							{
								Dialog::setControlBitmap(hWndDlg, IDC_STATIC_M2, IDB_M_QUESTION);
							}
							break;
					}

					if(values->type == Dialog::MessageDialog::TYPE::M_QUESTION)
					{
						ShowWindow(GetDlgItem(hWndDlg, IDOK), SW_HIDE);
					}
					else
					{
						ShowWindow(GetDlgItem(hWndDlg, IDYES), SW_HIDE);
						ShowWindow(GetDlgItem(hWndDlg, IDNO), SW_HIDE);
					}
				}
			}
			break;

		case WM_CTLCOLORDLG:
			{
				return reinterpret_cast<INT_PTR>(GetStockObject(WHITE_BRUSH));
			}
			break;

		case WM_CTLCOLORSTATIC: 
			{
				HDC hDC = reinterpret_cast<HDC>(wParam);
				HWND hControl = reinterpret_cast<HWND>(lParam);

				if(hControl == GetDlgItem(hWndDlg, IDC_STATIC_M1))
				{
					SetTextColor(hDC, RGB(24, 122, 201));
				}
				else
				{
					SetTextColor(hDC, RGB(77, 77, 102));
				}

				return reinterpret_cast<INT_PTR>(GetStockObject(WHITE_BRUSH));
			}
			break;

		case WM_COMMAND:
			{
				EndDialog(hWndDlg, LOWORD(wParam));
			}
			break;

		case WM_DESTROY:
			{
				Dialog::deleteControlFont(hWndDlg, IDC_STATIC_M1);
				Dialog::deleteControlFont(hWndDlg, IDC_STATIC_M3);

				Dialog::deleteControlBitmap(hWndDlg, IDC_STATIC_M2);
			}
			break;
	}

	return FALSE;
}

VOID Dialog::setControlBitmap(HWND hWnd, UINT control, USHORT resourceBitmap)
{
	HBITMAP hBitmap = reinterpret_cast<HBITMAP>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceBitmap), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE));

	if(hBitmap != NULL)
	{
		SendDlgItemMessage(hWnd, control, STM_SETIMAGE, IMAGE_BITMAP, reinterpret_cast<LPARAM>(hBitmap));
	}
}

VOID Dialog::deleteControlBitmap(HWND hWnd, UINT control)
{
	HBITMAP hBitmap = reinterpret_cast<HBITMAP>(SendDlgItemMessage(hWnd, control, STM_GETIMAGE, IMAGE_BITMAP, 0));

	if(hBitmap != NULL)
	{
		DeleteObject(hBitmap);
	}
}

HFONT Dialog::setControlFont(HWND hWnd, UINT control, BYTE height, USHORT weight)
{
	HFONT hFont = CreateFont(height,
							 0,
							 0,
							 0,
							 weight,
							 FALSE,
							 FALSE,
							 FALSE,
							 DEFAULT_CHARSET,
							 OUT_DEFAULT_PRECIS,
							 CLIP_DEFAULT_PRECIS,
							 DEFAULT_QUALITY,
							 DEFAULT_PITCH | FF_DONTCARE,
							 _T("Arial"));

	if(hFont != NULL)
	{
		SendDlgItemMessage(hWnd, control, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), MAKELPARAM(TRUE, 0));
	}

	return hFont;
}

VOID Dialog::deleteControlFont(HWND hWnd, UINT control)
{
	HFONT hFont = reinterpret_cast<HFONT>(SendDlgItemMessage(hWnd, control, WM_GETFONT, 0, 0));

	if(hFont != NULL)
	{
		DeleteObject(hFont);
	}
}

VOID Dialog::setTitle(_INOUT_(std::basic_string<TCHAR> & title), std::basic_string<TCHAR> append)
{
	if(!title.empty())
	{
		title.append(_T(" - "));
	}

	title.append(append);
}

HRESULT Dialog::dialog(CLSID key, _IN_(std::basic_string<TCHAR> & title), _IN_(std::vector<COMDLG_FILTERSPEC> & types), _OUT_(std::basic_string<TCHAR> & file))
{
	IFileDialog* dialog = NULL;

	HRESULT hResult = CoCreateInstance(key, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dialog));

	if(SUCCEEDED(hResult))
	{
		dialog->SetTitle(title.data());

		if(!types.empty())
		{
			dialog->SetFileTypes(types.size(), types.data());
		}

		DWORD flags;
		dialog->GetOptions(&flags);
		dialog->SetOptions(flags | FOS_STRICTFILETYPES);

		hResult = dialog->Show(NULL);

		if(SUCCEEDED(hResult))
		{
			IShellItem* item = NULL;
			hResult = dialog->GetResult(&item);

			if(SUCCEEDED(hResult))
			{
				LPTSTR filePath = NULL;
				hResult = item->GetDisplayName(SIGDN_FILESYSPATH, &filePath);

				if(SUCCEEDED(hResult))
				{
					file = filePath;

					CoTaskMemFree(filePath);
				}

				item->Release();
			}
		}

		dialog->Release();
	}

	return hResult;
}