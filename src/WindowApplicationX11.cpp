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

$Id: WindowApplicationX11.cpp,v 1.2 2002/11/29 09:31:47 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/config.h>
#ifdef HAVE_X11
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Interface.hpp>
#include <nmo/WindowApplication.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "interface/X11Win.hpp"
#include "interface/IDisplayX11.hpp"

//-------------------------------------------------
WindowApplication::WindowApplication(const UStringz &N): Application(N)
{
	XInitThreads();
}


//-------------------------------------------------
WindowApplication::~WindowApplication()
{ }


//-------------------------------------------------
void WindowApplication::SetName(const UStringz&NAME)
{
	ListIterator<IWindow::Link> list(IWindow::Link::handles);
	if (list.Count() > 0) {
		XTextProperty prop_name;
		Stringz txt = NAME;
		XGetWMName(X11Win::display, Window(list[1].handle), &prop_name);
		prop_name.value = (unsigned char*)(_CHAR*)txt;
		prop_name.format = 8;
		prop_name.nitems = txt.Len() + 1;
		XSetWMName(X11Win::display, Window(list[1].handle), &prop_name);
	}
}


//-------------------------------------------------
_BOOL WindowApplication :: OpenDisplayEnv()
{
	X11Win::eventloopstarted = TRUE;
	return X11Win::OpenScreen();
}

//-------------------------------------------------
void WindowApplication :: CloseDisplayEnv()
{
	X11Win::eventloopstarted = FALSE;
	//XSync(X11Win::display, True);
	//XCloseDisplay(X11Win::display);
}

//-------------------------------------------------
void WindowApplication :: ProcessNextEvent(IWindow*WINDOW)
{
	IWindow *wnd;
	XEvent	event;
	_BOOL	closequit = FALSE;

	XNextEvent(X11Win::display, &event);

	wnd = IWindow::Link::Handle2Nmo((IHandle)event.xany.window);
	if (wnd == NULL) { return; }

	switch (event.type) {
	case DestroyNotify:
		{
			wnd->Display().DestroyBuffers();
			wnd->EventDestroy();
			ListIterator<IWindow::Link> list(IWindow::Link::handles);
			if (!(closequit)) {
				closequit = (event.xany.window == IHANDLE(list[1].handle));
				IWindow::Link::Drop(*wnd);
				if (closequit) {
					CloseAllWindows();
				}
			}
			else {
				IWindow::Link::Drop(*wnd);
			}
			mQuit = IWindow::Link::handles.Count() == 0;
		}
		break;
	case ConfigureNotify:
		{
			while (XCheckWindowEvent(X11Win::display,
									event.xany.window,
									StructureNotifyMask,
									&event)) { }
			if ((wnd->oldRect.width != (_DWORD)event.xconfigure.width) ||
			(wnd->oldRect.height != (_DWORD)event.xconfigure.height)) {
				wnd->oldRect.width = (_DWORD)event.xconfigure.width;
				wnd->oldRect.height = (_DWORD)event.xconfigure.height;
				wnd->EventResize();
				wnd->ResizeDisplay();
				IRect r(0,0,event.xconfigure.width,event.xconfigure.height);
				wnd->EventDraw(r);
				wnd->RefreshDisplay(r);
			}
			if ((wnd->oldRect.left != event.xconfigure.x) ||
				(wnd->oldRect.top != event.xconfigure.y)) {
				wnd->oldRect.left = event.xconfigure.x;
				wnd->oldRect.top = event.xconfigure.y;
				wnd->EventMove();
			}
		}
		break;
	case Expose:
		{
			XRectangle rect;
			Region region = XCreateRegion();
			do {
				rect.x = (short)event.xexpose.x;
				rect.y = (short)event.xexpose.y;
				rect.width = (unsigned short)event.xexpose.width;
				rect.height = (unsigned short)event.xexpose.height;
				XUnionRectWithRegion(&rect, region, region);
			}
			while (XCheckWindowEvent(X11Win::display,
									event.xany.window,
									ExposureMask,
									&event));
			if (!XCheckTypedWindowEvent(X11Win::display,
									   event.xany.window,
										StructureNotifyMask,
										&event)) {
				XClipBox(region, &rect);
					IRect r;
				r.left = rect.x;
				r.top = rect.y;
				r.width = rect.width;
				r.height = rect.height;
				wnd->EventDraw(r);
				wnd->RefreshDisplay(r);
				XDestroyRegion(region);
			}
		}
		break;
	case ClientMessage:
		if ((event.xclient.format == 32) &&
			(event.xclient.message_type == XInternAtom(X11Win::display,
			"WM_PROTOCOLS", FALSE)) &&
			(event.xclient.data.l[0] = (long)XInternAtom(X11Win::display,
			"WM_DELETE_WINDOW", FALSE)))
		{
			if (wnd->EventQueryDestroy()) {
				XDestroyWindow(X11Win::display, event.xany.window);
			}
		}
		break;
	case FocusIn:
		wnd->EventGotFocus();
		break;
	case FocusOut:
		wnd->EventLostFocus();
		break;
	case MapNotify:
		//wnd->EventShow();
		break;
	case UnmapNotify:
		//wnd->EventHide();
		break;
	default:
		if ((WINDOW != NULL) && (WINDOW != wnd)) {
			return;
		}
		switch (event.type) {
		case MotionNotify:
			{
				IMouse::Button button;
				switch (event.xbutton.button) {
				case Button1:
					button = IMouse::LEFT;
					break;
				case Button2:
					button = IMouse::MIDDLE;
				break;
				case Button3:
					button = IMouse::RIGHT;
					break;
				default:
					button = IMouse::NONE;
					break;
				}
				IMouse::XPos() = event.xmotion.x_root;
				IMouse::YPos() = event.xmotion.y_root;
				wnd->EventMouseMove(button,
									event.xmotion.x,
									event.xmotion.y);
			}
			break;
		case ButtonPress:
		case ButtonRelease:
			{
				IMouse::Button button;
				switch (event.xbutton.button) {
				case Button1:
					button = IMouse::LEFT;
					break;
				case Button2:
					button = IMouse::MIDDLE;
					break;
				case Button3:
					button = IMouse::RIGHT;
					break;
				default:
					button = IMouse::NONE;
					break;
				}
				IMouse::XPos() = event.xbutton.x_root;
				IMouse::YPos() = event.xbutton.y_root;
				if (event.type == ButtonPress)
					wnd->EventMouseDown(button, event.xbutton.x,
										event.xbutton.y);
				else
					wnd->EventMouseUp(button, event.xbutton.x,
									event.xbutton.y);
			}
			break;
		case KeyPress:
			{
				IKeyb::Key key = XLookupKeysym(&event.xkey, 0);
				if (key < 0xFF00)
					XLookupString(&event.xkey, (char*)&key, 2, NULL, NULL);
				wnd->Keyb().SetEvent(&event.xkey);
				wnd->Keyb().SetKey(key);
				wnd->EventKeybDown(key);
			}
			break;
		case KeyRelease:
			{
				IKeyb::Key key = XLookupKeysym(&event.xkey, 0);
				wnd->Keyb().SetEvent(&event.xkey);
				wnd->Keyb().UnsetKey(key);
				wnd->EventKeybUp(key);
			}
			break;
		default:
			//Debug(dprintf("WindowApplication: unhandled event: %d for %d\n", event.type, event.xany.window);)
			break;
		}
		break;
	}
}


//-------------------------------------------------
_BOOL WindowApplication::Create(IWindow::Link &link,
								IWindow::WindowType TYPE,
								const Stringz&)
{
	XSetWindowAttributes attr;
	attr.background_pixmap = None;
	attr.background_pixel = WhitePixel(X11Win::display, X11Win::screen_num);
	attr.border_pixmap = None;
	attr.border_pixel = WhitePixel(X11Win::display, X11Win::screen_num);
	attr.event_mask = 0;

	unsigned long valuemask= CWBackPixel |
							 CWBorderPixel |
							 CWEventMask;
	switch (TYPE)
	{
	case IWindow::NORMAL:
		break;
	case IWindow::SIZEABLE:
		break;
	case IWindow::DIALOG:
		break;
	case IWindow::POPUP:
		attr.save_under = False;
		attr.override_redirect = True;
		valuemask |= CWSaveUnder;
		valuemask |= CWOverrideRedirect;
		break;
	};

	link.handle = (_PTR) XCreateWindow(X11Win::display,
								RootWindow(X11Win::display, X11Win::screen_num),
								0, 0,
								1, 1,
								0,
								CopyFromParent,
								InputOutput,
								CopyFromParent,
								valuemask,
								&attr);
	if (link.handle == NULL)
	{
		Debug(dprintf("WindowApplication: CreateWindowEx\n");)
		return FALSE;
	}
	link.nmo->oldRect.left = 0;
	link.nmo->oldRect.top = 0;
	link.nmo->oldRect.width = 1;
	link.nmo->oldRect.height = 1;

	XWMHints *hints = XAllocWMHints();
	if (hints != NULL) {
		hints->flags = StateHint;
		hints->initial_state = NormalState;
		XSetWMHints(X11Win::display, IHANDLE(link.handle), hints);
		XFree(hints);
	}

	XSizeHints *size_hints =  XAllocSizeHints();
	if (size_hints != NULL) {
		size_hints->x = size_hints->y = 0;
		size_hints->width = size_hints->height = 10;
		size_hints->flags = PSize | PPosition;
		XSetWMNormalHints(X11Win::display, IHANDLE(link.handle), size_hints);
		XFree(size_hints);
	}

	Atom protocols[2];
	protocols[0] = XInternAtom(X11Win::display, "WM_SAVE_YOURSELF", False);
	protocols[1] = XInternAtom(X11Win::display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(X11Win::display, IHANDLE(link.handle), protocols, 2);

	XSelectInput(X11Win::display, IHANDLE(link.handle),
					KeyPressMask | StructureNotifyMask |
					FocusChangeMask | ExposureMask |
					ButtonPressMask | ButtonReleaseMask |
					PointerMotionMask | FocusChangeMask |
					ButtonMotionMask);

	IDisplayX11 *DISP = new IDisplayX11(IDisplay::DISPLAY_DOUBLE, link.handle);
	link.nmo->display = (IDisplay*) DISP;
	return TRUE;
}

#endif
