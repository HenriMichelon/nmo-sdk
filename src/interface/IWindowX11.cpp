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

$Id: IWindowX11.cpp,v 1.2 2002/11/29 09:32:25 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/config.h>
#ifdef HAVE_X11
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Interface.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "X11Win.hpp"
#include "IDisplayX11.hpp"
#include "nmo/WindowApplication.hpp"


//------------------------------------------------------------
IWindow::IWindow()
{
	display = NULL;
	handle = NULL;
	closing = FALSE;
}


//------------------------------------------------------------
IWindow::~IWindow()
{
	if (display != NULL) { 
		delete display; 
		display = NULL;
	}
	Link::Drop(*this);
}


//------------------------------------------------------------
void IWindow::SetTitle(const UStringz&TEXT)
{
	Stringz NAME = TEXT;
	XTextProperty text;
	_CHAR *name = NAME;
	XStringListToTextProperty(&name, 1, &text);
	XSetWMName(X11Win::display, IHANDLE(handle), &text);
}


//------------------------------------------------------------
UStringz IWindow::Title() const
{
	XTextProperty text;
	if (XGetWMName(X11Win::display, IHANDLE(handle), &text))
		return UStringz((_CHAR*)text.value);
	else
		return "";
}


//------------------------------------------------------------
_BOOL IWindow::Visible() const
{
	WindowApplication &app = (WindowApplication&)Application::App();
	if (closing || app.Terminated()) {
		return FALSE;
	}
	else {
		XWindowAttributes attribs;
		XGetWindowAttributes(X11Win::display, IHANDLE(handle), &attribs);
		return (attribs.map_state == IsViewable);
	}
}


//------------------------------------------------------------
void IWindow::Show(_BOOL S)
{
	if (S) {
		EventShow();
		XMapWindow(X11Win::display, IHANDLE(handle));
	}
	else {
		XUnmapWindow(X11Win::display, IHANDLE(handle));
		EventHide();
	}
}


//------------------------------------------------------------
void IWindow::Close()
{
	if (!closing) {
		if (EventQueryDestroy()) {
			closing = TRUE;
			XDestroyWindow(X11Win::display, IHANDLE(handle));
		}
	}
}


//------------------------------------------------------------
_LONG IWindow::Top() const
{
	int rx, ry;
	Window junk;
	XWindowAttributes attribs;

	XGetWindowAttributes(X11Win::display, IHANDLE(handle), &attribs);
	XTranslateCoordinates(X11Win::display, IHANDLE(handle), attribs.root,
						  -attribs.border_width,
						  -attribs.border_width,
						  &rx, &ry, &junk);
	return ry;
}


//------------------------------------------------------------
_LONG IWindow::Left() const
{
	int rx, ry;
	Window junk;
	XWindowAttributes attribs;

	XGetWindowAttributes(X11Win::display, IHANDLE(handle), &attribs);
	XTranslateCoordinates(X11Win::display, IHANDLE(handle), attribs.root,
						  -attribs.border_width,
						  -attribs.border_width,
						  &rx, &ry, &junk);
	return rx;
}


//------------------------------------------------------------
void IWindow::SetPos(_LONG L, _LONG T)
{
	XWindowChanges changes;
	changes.x = L;
	changes.y = T;
	XConfigureWindow(X11Win::display, IHANDLE(handle), CWX | CWY, &changes);
}


//------------------------------------------------------------
_DWORD IWindow::Width() const
{
	XWindowAttributes attribs;
	XGetWindowAttributes(X11Win::display, IHANDLE(handle), &attribs);
	return attribs.width;
}


//------------------------------------------------------------
_DWORD IWindow::Height() const
{
	XWindowAttributes attribs;
	XGetWindowAttributes(X11Win::display, IHANDLE(handle), &attribs);
	return attribs.height;
}


//------------------------------------------------------------
void IWindow::SetSize(_DWORD W, _DWORD H)
{
	XWindowChanges changes;
	changes.width = W;
	changes.height = H;
	XConfigureWindow(X11Win::display, IHANDLE(handle), CWWidth | CWHeight, &changes);
	if (!X11Win::eventloopstarted) { ResizeDisplay(); }
}


//------------------------------------------------------------
IRect IWindow::Rect() const
{
	IRect res;
	Window junk;
	XWindowAttributes attribs;

	int left;
	int top;
	XGetWindowAttributes(X11Win::display, IHANDLE(handle), &attribs);
	XTranslateCoordinates(X11Win::display, IHANDLE(handle), attribs.root,
						  -attribs.border_width,
						  -attribs.border_width,
						  &left, &top, &junk);
	res.left = left;
	res.top = top;
	res.width = attribs.width;
	res.height = attribs.height;
	return res;
}


//------------------------------------------------------------
_BOOL IWindow::HasFocus() const
{
	Window 	focus;
	int		revert_to;
	XGetInputFocus(X11Win::display, &focus, &revert_to);
	return (focus == IHANDLE(handle));
}


//------------------------------------------------------------
void IWindow::GiveFocus()
{
	XSetInputFocus(X11Win::display, IHANDLE(handle), RevertToNone, CurrentTime);
}


//------------------------------------------------------------
void IWindow :: SetBgColor (IRGBColor &COLOR)
{
    XColor exact_def;
    Colormap default_cmap = DefaultColormap(X11Win::display, X11Win::screen_num);
	Stringz fmt;
	fmt.Format("#%02x%02x%02x", COLOR.red, COLOR.green, COLOR.blue);
    if (XParseColor (X11Win::display,
                      default_cmap,
                       fmt,
                       &exact_def)) {
    	XAllocColor(X11Win::display, default_cmap, &exact_def);
		XSetWindowBackground(X11Win::display, IHANDLE(handle), exact_def.pixel);
		XSetWindowBorder(X11Win::display, IHANDLE(handle), exact_def.pixel); 
	}
}


#endif
