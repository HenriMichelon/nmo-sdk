/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.x-tech.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1998-2002 Henri Michelon

$Id: WindowApplicationWin32.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
-------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <nmo/Thread.hpp>
# include <nmo/Interface.hpp>
#endif
#include <nmo/WindowApplication.hpp>
#include "interface/WinApp.hpp"
#include "interface/SimpleDIB.hpp"
#include "interface/IDisplayDIB.hpp"
#include "interface/IDisplayDirectDraw.hpp"

static HINSTANCE hInst;

//-------------------------------------------------
void win_main_init(HINSTANCE INST)
{
	hInst = INST;
	Debug(dopen("debug.txt");)
	// Parse command line arguments
	// to create application arguments list
	Stringz arg;
	_CHAR* cline = GetCommandLine();
	for (_DWORD i=0; i<strlen(cline); i++) {
		if (cline[i] == ' ') {
			Application::Argv().Add(new Stringz(arg));
			arg = "";
		}
		else
			arg += cline[i];
	}
	Application::Argv().Add(new Stringz(arg));
	ListIterator<Stringz> list(Application::Argv());
	Application::SetPath(File::FilePath(list[1]));
}


//-------------------------------------------------
WindowApplication::WindowApplication(const UStringz &N): Application(N)
{
	mWindowApplicationAbstract = new WinApp;
	WINAPP(lpDD) = NULL;
	WINAPP(hInst) = hInst;
	// Create default font for all windows
	WINAPP(font) = CreateFont(-6, -6, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
	IScreen::SetSize(GetSystemMetrics(SM_CXSCREEN), 
					GetSystemMetrics(SM_CYSCREEN));
}


//-------------------------------------------------
WindowApplication::~WindowApplication()
{
	delete (WinApp*)mWindowApplicationAbstract;
}


//-------------------------------------------------
void WindowApplication::SetName(const UStringz&NAME) {}


//-------------------------------------------------
_BOOL WindowApplication :: OpenDisplayEnv() 
{ 
	WINAPP(quit) = FALSE;
	return TRUE; 
}

//-------------------------------------------------
void WindowApplication :: CloseDisplayEnv() {}

//-------------------------------------------------
void WindowApplication :: ProcessNextEvent(IWindow*WINDOW)
{
	HWND handle = NULL;
	if (WINDOW != NULL) {
		handle = (HWND)WINDOW->Handle();
	}
	MSG msg;
	if (GetMessage(&msg, handle, 0, 0) >= 0) {
		mQuit = (msg.message == WM_QUIT);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}


//-------------------------------------------------
_BOOL WindowApplication::Create(IWindow::Link &link,
								IWindow::WindowType TYPE,
								const Stringz&DRIVER)
{
	HWND		parenthwnd = NULL;
	Stringz		windowName;

	link.nmo->style = WS_CAPTION;

	// The first created window is the parent of all
	// others windows
	if (IWindow::Link::handles.Count() > 0) {
		ListIterator<IWindow::Link> list(IWindow::Link::handles);
		parenthwnd = IHANDLE(list[1].handle);
	}
	else {
		// Create a new window class (the same for all windows)
		WINAPP(className) = "NMOwndclass";
		WINAPP(wndclass.style)= CS_HREDRAW | CS_VREDRAW;
		WINAPP(wndclass.lpfnWndProc)= WNDPROC(WinApp::WindowProc);
		WINAPP(wndclass.cbClsExtra)= 0;
		WINAPP(wndclass.cbWndExtra)= 0;
		WINAPP(wndclass.hInstance)= WINAPP(hInst);
		WINAPP(wndclass.hIcon) = LoadIcon(WINAPP(hInst), IDI_APPLICATION);
		WINAPP(wndclass.hCursor)= LoadCursor(NULL, IDC_ARROW );
		WINAPP(wndclass.hbrBackground) = NULL; //(HBRUSH) COLOR_WINDOW;
		WINAPP(wndclass.lpszMenuName) = NULL;
		WINAPP(wndclass.lpszClassName) = WINAPP(className);
    	if (!RegisterClass(&WINAPP(wndclass))) {
			Debug(dprintf("WindowApplication: RegisterClass: %d\n", GetLastError());)
			return FALSE;
		}
		link.nmo->style |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	}

	switch (TYPE) {
	case IWindow::SIZEABLE:
		link.nmo->style |= WS_SIZEBOX | WS_SYSMENU;
		break;
	case IWindow::DIALOG:
		link.nmo->style |= WS_DLGFRAME;
		break;
	case IWindow::POPUP:
		link.nmo->style = WS_POPUP;
		break;
	default: //case IWindow::NORMAL:
		link.nmo->style = WS_MINIMIZEBOX | WS_SYSMENU;
		break;
	};

	link.handle = CreateWindowEx(
				0,
				(_CHAR*)WINAPP(className),
				NULL,
				link.nmo->style,
				0, 0, 100, 100,
				parenthwnd,
				NULL,
				WINAPP(hInst),
				NULL);
	if (!link.handle) {
		Debug(dprintf("WindowApplication: CreateWindowEx: %d\n", GetLastError());)
		return FALSE;
	}
	
	link.nmo->handle = link.handle;

	if ((DRIVER == "") || (DRIVER == "DIB")) {
		IDisplayDIB *DIB = new IDisplayDIB(IDisplay::DISPLAY_DOUBLE, IHANDLE(link.handle), WINAPP(hInst));
		link.nmo->display = (IDisplay*) DIB;
	}
	else if (DRIVER == "DirectDraw") {
		if (WINAPP(lpDD) == NULL) {
   			HRESULT ddrval = DirectDrawCreate(NULL, &WINAPP(lpDD), NULL );
   			if(ddrval != DD_OK ) {
				Debug(dprintf("WindowApplication(): unable to create main DirectDraw object\n");)
   				return FALSE;
   			}

			ddrval = WINAPP(lpDD)->SetCooperativeLevel(IHANDLE(link.handle),
														DDSCL_NORMAL);
   			if (ddrval != DD_OK ) {
				Debug(dprintf("WindowApplication(): unable to set DirectDraw cooperative level #%x\n", ddrval);)
				WINAPP(lpDD->Release());
   				return FALSE;
   			}
		}

		IDisplayDirectDraw *DIB = new IDisplayDirectDraw(IDisplay::DISPLAY_DOUBLE,
														IHANDLE(link.handle),
														WINAPP(hInst),
														WINAPP(lpDD));
		link.nmo->display = (IDisplay*) DIB;
	}
	else {
		Debug(dprintf("WindowApplication: Unknown display driver\n");)
		return FALSE;
	}
	return TRUE;
}
#endif
