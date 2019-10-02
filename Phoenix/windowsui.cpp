#include "windowsui.hpp"

ClassProperty::ClassProperty(_IN_(std::basic_string<TCHAR> & name), UINT style, HBRUSH backgroundColor, WNDPROC windowProcedure):
style(style),
backgroundColor(backgroundColor),
windowProcedure(windowProcedure)
{
	this->name = name.substr(0, ClassProperty::nameSize);
}

ClassProperty::~ClassProperty()
{
}

WindowProperty::WindowProperty(_IN_(std::basic_string<TCHAR> & name), DWORD style, _IN_(ARRAY2REF(SHORT, position)), _IN_(ARRAY2REF(SHORT, size)), USHORT id, USHORT parentId, BOOL show):
style(style),
id(id),
parentId(parentId),
show(show)
{
	this->name = name.substr(0, WindowProperty::nameSize);
	this->position[mCOORDINATE::X] = position[mCOORDINATE::X];
	this->position[mCOORDINATE::Y] = position[mCOORDINATE::Y];
	this->size[mSIZE::WIDTH] = size[mSIZE::WIDTH];
	this->size[mSIZE::HEIGHT] = size[mSIZE::HEIGHT];
}

WindowProperty::~WindowProperty()
{
}

StatusBarProperty::StatusBarProperty(_IN_(std::basic_string<TCHAR> & content), USHORT id, USHORT parentId, COLORREF backgroundColor):
id(id),
parentId(parentId),
backgroundColor(backgroundColor)
{
	this->content = content.substr(0, StatusBarProperty::contentSize);
}

StatusBarProperty::~StatusBarProperty()
{
}

Window::Window(HWND hWnd, USHORT id, HDC hDC, HGLRC hRC):
hWnd(hWnd),
id(id),
hDC(hDC),
hRC(hRC)
{
}

Window::~Window()
{
}

WindowsUI::WindowsUI():
hInstance(NULL)
{
}

WindowsUI::~WindowsUI()
{
	for(std::vector<Window*>::const_iterator i = this->windows.begin(), j = this->windows.end(); i != j; i++)
	{
		delete *i;
	}

	this->windows.clear();
}

VOID WindowsUI::getScreenSize(_OUT_(ARRAY2REF(SHORT, size)))
{
	size[mCOORDINATE::X] = GetSystemMetrics(SM_CXSCREEN);
	size[mCOORDINATE::Y] = GetSystemMetrics(SM_CYSCREEN);
}

VOID WindowsUI::createUI(HINSTANCE hInstance, _IN_(ClassProperty & cp), _IN_(WindowProperty & wp), _IN_(StatusBarProperty & sp))
{
	this->hInstance = hInstance;

	if(WindowsUI::registerClass(hInstance, cp) != 0)
	{
		if(this->createWindow(hInstance, cp.name, wp))
		{
			HWND statusBar = CreateStatusWindow(WS_CHILD | WS_VISIBLE, sp.content.data(), this->getHwnd(sp.parentId), sp.id);

			if(statusBar != NULL)
			{
				Window* window = new Window(statusBar, sp.id);

				if(window != NULL)
				{
					ExceptionHandler::push_back<Window*>(this->windows, window);

					SendMessage(window->hWnd, SB_SETBKCOLOR, 0, sp.backgroundColor);
				}
			}

			WindowsUI::getWindowMessages();
		}
	}
}

HWND WindowsUI::getHwnd(USHORT id)
{
	Window* window = this->getWindowById(id);

	if(window != NULL)
	{
		return window->hWnd;
	}

	return NULL;
}

BOOL WindowsUI::createGraphicWindow(_IN_(WindowProperty & wp))
{
	BOOL returnValue = FALSE;

	HWND hWndParent = this->getHwnd(wp.parentId);

	if(hWndParent != NULL)
	{
		std::basic_string<TCHAR> className;
		className.resize(ClassProperty::nameSize);

		if(GetClassName(hWndParent, &className.front(), className.capacity()) != 0)
		{
			if(this->createWindow(this->hInstance, className, wp))
			{
				Window* window = this->getWindowById(wp.id);

				if(window != NULL)
				{
					window->hDC = GetDC(window->hWnd);

					if(WindowsUI::setPixelFormat(window->hDC))
					{
						window->hRC = wglCreateContext(window->hDC);

						if(window->hRC != NULL)
						{
							if(wglMakeCurrent(window->hDC, window->hRC))
							{
								if(glewInit() == GLEW_OK)
								{
									if(!WGLEW_ARB_multisample)
									{
										returnValue = TRUE;
									}
									else
									{
										this->destroyWindow(wp.id);

										if(this->createWindow(this->hInstance, className, wp))
										{
											window = this->getWindowById(wp.id);

											returnValue = WindowsUI::setMsAA(window);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return returnValue;
}

BOOL WindowsUI::moveBack2Front(USHORT id)
{
	//HGLRC wglGetCurrentContext();

	Window* window = this->getWindowById(id);

	if(window != NULL)
	{
		if(window->hDC != NULL && window->hRC != NULL)
		{
			return SwapBuffers(window->hDC);
		}
	}

	return FALSE;
}

VOID WindowsUI::registerHotkey(HWND hWnd, INT id, UINT systemKey, UINT virtualKey)
{
	if(RegisterHotKey(hWnd, id, systemKey, virtualKey))
	{
		ExceptionHandler::push_back<INT>(this->hotkey, id);
	}
}

VOID WindowsUI::destroyWindow(USHORT id)
{
	std::vector<Window*>::iterator location = std::find_if(this->windows.begin(), this->windows.end(), ComparatorWindow(id));

	if(location != this->windows.end())
	{
		Window* window = *location;

		if(window != NULL)
		{
			WindowsUI::releaseContext(window);

			DestroyWindow(window->hWnd);

			delete window;

			this->windows.erase(location);
		}
	}
}

VOID WindowsUI::release()
{
	HWND hWnd = this->getHwnd(mWINDOW::MAIN);

	for(std::vector<INT>::const_iterator i = this->hotkey.begin(), j = this->hotkey.end(); i != j; i++)
	{
		UnregisterHotKey(hWnd, *i);
	}

	this->hotkey.clear();

	for(std::vector<Window*>::iterator i = this->windows.begin(), j = this->windows.end(); i != j; i++)
	{
		WindowsUI::releaseContext(*i);
	}
}

ATOM WindowsUI::registerClass(HINSTANCE hInstance, _IN_(ClassProperty & cp))
{
	WNDCLASSEX wcex;

	SecureZeroMemory (&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = cp.style;
	wcex.lpfnWndProc = cp.windowProcedure;
	wcex.cbClsExtra = NULL;
	wcex.cbWndExtra = NULL;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = cp.backgroundColor;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = cp.name.data();

	return RegisterClassEx(&wcex);
}

BOOL WindowsUI::setPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pfd;

	SecureZeroMemory(&pfd, sizeof(pfd));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	INT format = ChoosePixelFormat(hDC, &pfd);

	return SetPixelFormat(hDC, format, &pfd);
}

VOID WindowsUI::releaseContext(Window* window)
{
	if(window != NULL)
	{
		if(window->hDC != NULL)
		{
			if(window->hRC != NULL)
			{
				if(wglMakeCurrent(window->hDC, NULL))
				{
					wglDeleteContext(window->hRC);
				}
			}

			ReleaseDC(window->hWnd, window->hDC);
		}
	}
}

BOOL WindowsUI::setMsAA(Window* window)
{
	BOOL returnValue = FALSE;

	if(window != NULL)
	{
		window->hDC = GetDC(window->hWnd);

		INT attributes[] = {WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
							WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
							WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
							WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
							WGL_COLOR_BITS_ARB, 32,
							WGL_DEPTH_BITS_ARB, 24,
							WGL_STENCIL_BITS_ARB, 8,
							WGL_SAMPLE_BUFFERS_ARB, 1,
							WGL_SAMPLES_ARB, 2,
							0};

		INT pixelFormat = 0;
		UINT formatCount = 0;
		wglChoosePixelFormatARB(window->hDC, attributes, NULL, 1, &pixelFormat, &formatCount);

		if(pixelFormat > 0)
		{
			PIXELFORMATDESCRIPTOR pfd;
			DescribePixelFormat(window->hDC, pixelFormat, sizeof(pfd), &pfd);

			if(SetPixelFormat(window->hDC, pixelFormat, &pfd))
			{
				INT context[] = {WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, 0};

				window->hRC = wglCreateContextAttribsARB(window->hDC, 0, context);

				if(window->hRC != NULL)
				{
					if(wglMakeCurrent(window->hDC, window->hRC))
					{
						returnValue = TRUE;
					}
				}
			}
		}
	}

	return returnValue;
}

BOOL WindowsUI::createWindow(HINSTANCE hInstance, _IN_(std::basic_string<TCHAR> & className), _IN_(WindowProperty & wp))
{
	HWND hWnd = NULL;
	HWND hWndParent = NULL;

	if(wp.parentId != 0)
	{
		hWndParent = this->getHwnd(wp.parentId);
	}

	hWnd = CreateWindowEx(0,
						  className.data(),
						  wp.name.data(),
						  wp.style,
						  wp.position[mCOORDINATE::X],
						  wp.position[mCOORDINATE::Y],
						  wp.size[mSIZE::WIDTH],
						  wp.size[mSIZE::HEIGHT],
						  (HWND) hWndParent,
						  (HMENU) NULL,
						  hInstance,
						  (LPVOID) NULL);

	if(hWnd != NULL)
	{
		Window* window = new Window(hWnd, wp.id);

		if(window != NULL)
		{
			ExceptionHandler::push_back<Window*>(this->windows, window);

			ShowWindow(window->hWnd, wp.show);

			return UpdateWindow(window->hWnd);
		}
	}

	return FALSE;
}

VOID WindowsUI::getWindowMessages()
{
	MSG msg;

	Window* mainWindow = WindowsUI::getWindowById(mWINDOW::MAIN);

	HACCEL hAccelerator = LoadAccelerators(this->hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(msg.message == WM_QUIT)
		{
			break;
		}

		if(hAccelerator != NULL)
		{
			TranslateAccelerator(mainWindow->hWnd, hAccelerator, &msg);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

Window* WindowsUI::getWindowById(_IN_(USHORT & id))
{
	std::vector<Window*>::iterator window = std::find_if(this->windows.begin(), this->windows.end(), ComparatorWindow(id));

	if(window != this->windows.end())
	{
		return *window;
	}

	return NULL;
}