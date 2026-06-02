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

	(c) 1998-2001 Henri Michelon

$Id: WinApp.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
----------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <nmo/Interface.hpp>
#endif
#include <nmo/WindowApplication.hpp>
#include "WinApp.hpp"


//-------------------------------------------------
// Window event handler. The same for ALL windows
//-------------------------------------------------
LRESULT CALLBACK WinApp::WindowProc(HWND hWnd, UINT message,
							WPARAM wParam, LPARAM lParam)
{
	IWindow *wnd = IWindow::Link::Handle2Nmo(hWnd);
	if (!wnd) return DefWindowProc(hWnd, message, wParam, lParam);

	switch (message)
	{
	case WM_DESTROY:
		wnd->EventDestroy();
		IWindow::Link::Handle2Nmo(hWnd);
        if (WINAPP(quit)) {
	     	IWindow::Link::Drop(*wnd);
		}
   		else {
			ListIterator<IWindow::Link> list(IWindow::Link::handles);
   		    WINAPP(quit) = (hWnd == list[1].handle);
       		IWindow::Link::Drop(*wnd);
			if (WINAPP(quit)) {
   		    	((WindowApplication&)Application::App()).CloseAllWindows();
				Application::App().Terminate();
				PostQuitMessage(0);
   	     	}
   		}
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		{
			IMouse::Button button = IMouse::NONE;
			_LONG x = LOWORD(lParam);
			_LONG y = HIWORD(lParam);
			if (wParam & MK_LBUTTON)
				button = IMouse::LEFT;
			else if (wParam & MK_MBUTTON)
				button = IMouse::MIDDLE;
			else if (wParam & MK_RBUTTON)
				button = IMouse::RIGHT;
			if (message == WM_MOUSEMOVE)
				wnd->EventMouseMove(button, x, y);
			else if ((message == WM_LBUTTONDOWN) ||
					(message == WM_MBUTTONDOWN) ||
					(message == WM_RBUTTONDOWN))
				wnd->EventMouseDown(button, x, y);
			else if ((message == WM_LBUTTONUP) ||
					(message == WM_MBUTTONUP) ||
					(message == WM_RBUTTONUP))
				wnd->EventMouseUp(button, x, y);
		}
		break;
	case WM_SETFOCUS:
		wnd->EventGotFocus();
		break;
	case WM_KILLFOCUS:
		wnd->EventLostFocus();
		break;
	case WM_KEYDOWN:
		wnd->EventKeybDown((IKeyb::Key)wParam);
		break;
	case WM_KEYUP:
		wnd->EventKeybUp((IKeyb::Key)wParam);
		break;
	case WM_SIZE:
		wnd->EventResize();
		wnd->ResizeDisplay();
		break;
	case WM_MOVE:
		wnd->EventMove();
		break;
	case WM_CLOSE:
		if (wnd->EventQueryDestroy()) { DestroyWindow(hWnd); }
		return 0;
	case WM_PAINT:
		{
			RECT R;
			IRect r;
			if (GetUpdateRect(hWnd, &R, FALSE))
			{
				r.left = R.left;
				r.top = R.top;
				r.width = R.right - R.left;
				r.height = R.bottom - R.top;
			}
			else
			{
				r.left = 0;
				r.top = 0;
				r.width = wnd->Width();
				r.height = wnd->Height();
			}
			wnd->EventDraw(r);
			wnd->RefreshDisplay(r);
		}
		break;
	case WM_SHOWWINDOW:
		if ((BOOL)wParam)
			wnd->EventShow();
		else
			wnd->EventHide();
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


#endif
