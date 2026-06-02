/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://www.nmo-sdk.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 2001 Henri Michelon

$Id: ICursorX11.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------------*/
#include <nmo/config.h>
#ifdef HAVE_X11
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Interface.hpp>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include "X11Win.hpp"


//------------------------------------------------------
void ICursor::SetCursor(CursorType CURSOR, IWindow *WND)
{
	Window wnd;
	if (WND == NULL) { 
		wnd = RootWindow(X11Win::display, X11Win::screen_num);
	}
	else {
		wnd = IHANDLE(WND->Handle());
	}
	switch (CURSOR) {
	case CUR_IBEAM: 
		//XDefineCursor(X11Win::display, wnd, XC_xterm);
		break;
	case CUR_CROSSHAIR: 
		//XDefineCursor(X11Win::display, wnd, XC_tcross);
		break;
	case CUR_HOURGLASS: 
		//XDefineCursor(X11Win::display, wnd, XC_watch);
		break;
	default: 
		XUndefineCursor(X11Win::display, wnd);
		return;
	}
}


//------------------------------------------------------
ICursor::CursorType ICursor::GetCursor(IWindow *)
{
	return CUR_DEFAULT;
}



//------------------------------------------------------
void ICursor::Show(_BOOL SHOW)
{
	if (SHOW) {
	}
	else {
	}
}

#endif
