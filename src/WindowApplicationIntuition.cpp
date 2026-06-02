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
   
	(c) 1998-2001 Henri Michelon 

$Id: WindowApplicationIntuition.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <clib/intuition_protos.h>
#include <clib/exec_protos.h>
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <nmo/WindowApplication.hpp>
using namespace NMO;
#include "interface/IDisplayAmiga.hpp"
#include "interface/WinIntuition.hpp"

#define GET_IMSG(M)     GetMsg((M))
#define REPL_IMSG(M)    ReplyMsg((struct Message *)(M))

//-------------------------------------------------
WindowApplication::WindowApplication(const UStringz &N): Application(N)
{
	if (!(WinIntuition::mainPort = (struct MsgPort *) CreateMsgPort())) {
		throw NMOException("Can't create message port for application");
	}
}


//-------------------------------------------------
WindowApplication::~WindowApplication()
{
	DeleteMsgPort(WinIntuition::mainPort);
}


//-------------------------------------------------
void WindowApplication::SetName(const UStringz&)
{ }


//-------------------------------------------------
_BOOL WindowApplication :: OpenDisplayEnv() { 
	return TRUE; 
}


//-------------------------------------------------
void WindowApplication :: CloseDisplayEnv() {}


//-------------------------------------------------
void WindowApplication :: ProcessNextEvent(IWindow*WINDOW)
{
	ASSERTMSG(WINDOW == NULL, "Modal windows not implemented");
	IntuiMessage    *msg;
	IWindow         *wnd;

	msg = (struct IntuiMessage *) GET_IMSG(WinIntuition::mainPort);
	if (msg == NULL) {
		WaitPort(WinIntuition::mainPort);
		msg = (struct IntuiMessage *) GET_IMSG(WinIntuition::mainPort);
	}
	if (msg == NULL) {
		return;
	}

	IHandle handle = msg->IDCMPWindow;
	wnd = IWindow::Link::Handle2Nmo((const IHandle&)handle);
	if (wnd == NULL) return;

	REPL_IMSG(msg);

	switch (msg->Class) {
	case IDCMP_CHANGEWINDOW:
		{	  
			_LONG top = handle->TopEdge + handle->BorderTop;
			_LONG left = handle->LeftEdge + handle->BorderLeft;
			if ( (top != wnd->newRect.top) &&
				 (left != wnd->newRect.left)) {
				wnd->newRect.top = top;
				wnd->newRect.left = left;
				wnd->EventMove();
			}
		}	
		break;
	case IDCMP_CLOSEWINDOW:
		if (wnd->EventQueryDestroy()) {
			wnd->Close();
			
		}
		break;
	case IDCMP_ACTIVEWINDOW:
		wnd->active = TRUE;
		wnd->EventGotFocus();
		break;
	case IDCMP_INACTIVEWINDOW:
		wnd->active = FALSE;
		wnd->EventLostFocus();
		break;
	case IDCMP_RAWKEY:
		{
			IKeyb::Key key = wnd->keyb.RawKey(msg);
			wnd->keyb.SetKey(key);
			wnd->EventKeybDown(key);
		}
	break;
	case IDCMP_REFRESHWINDOW:
		{
			IRect r;
			r.width = wnd->Width();
			r.height = wnd->Height();
			wnd->EventDraw(r);
			wnd->RefreshDisplay(r);
		}
	break;
	case IDCMP_NEWSIZE:
		wnd->newRect.width = handle->Width - 
							handle->BorderLeft - 
							handle->BorderRight;
		wnd->newRect.height = handle->Height -
							handle->BorderTop - 
							handle->BorderBottom;
		wnd->EventResize();
		wnd->ResizeDisplay();
		break;
	case IDCMP_MOUSEMOVE:
		{
			IMouse::XPos() = msg->MouseX + handle->LeftEdge;
			IMouse::YPos() = msg->MouseY + handle->TopEdge;
			if ((msg->MouseX > handle->BorderLeft) &&
				(msg->MouseY > handle->BorderTop) &&
				(msg->MouseX < (handle->BorderLeft + wnd->Width())) &&
				(msg->MouseY < (handle->BorderTop + wnd->Height()))) {
				wnd->EventMouseMove(IMouse::NONE, 
									msg->MouseX - handle->BorderLeft,
									msg->MouseY - handle->BorderTop);
			}
		}
		break;
	case IDCMP_MOUSEBUTTONS:
		{
			IMouse::XPos() = msg->MouseX + handle->LeftEdge;
			IMouse::YPos() = msg->MouseY + handle->TopEdge;
			if ((msg->MouseX > handle->BorderLeft) &&
				(msg->MouseY > handle->BorderTop) &&
				(msg->MouseX < (handle->BorderLeft + wnd->Width())) &&
				(msg->MouseY < (handle->BorderTop + wnd->Height()))) {
				_DWORD x = msg->MouseX - handle->BorderLeft;
				_DWORD y = msg->MouseY - handle->BorderTop;
				if ((msg->Code & 0xF0) == 0x60)
					wnd->EventMouseDown(IMouse::LEFT, x, y);
				else
					wnd->EventMouseUp(IMouse::LEFT, x, y);
			}
		}
		break;
	}
}



//-------------------------------------------------
_BOOL WindowApplication::Create(IWindow::Link &link,
                                IWindow::WindowType TYPE,
								const Stringz&)
{
	IDisplayAmiga *display = IDisplayAmiga::Create();
	if (!display) {
		dprintf("WindowApplication: Create display\n");
		return FALSE;
	}

	if (!display->OpenWBScreen()) {
		dprintf("WindowApplication: Open screen");
		return FALSE;
	}

	link.nmo->display = (IDisplay*) display;
	link.nmo->type = TYPE;
	link.nmo->link = &link;
	link.handle = NULL;
	return TRUE;
}
        
#endif
