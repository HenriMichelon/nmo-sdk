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

	(c) 2000-2002 Henri Michelon 

$Id: WindowApplicationPhoton.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifdef __QNX__
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <nmo/WindowApplication.hpp>
#include <Pt.h>
#include "interface/IDisplayPhoton.hpp"
#include <Ap.h>
using namespace NMO;


void nmo_raw_callback (PtWidget_t *widget, 
					   PhTile_t *damage)
{
	PtWidget_t *window = PtWidgetParent(widget);
	if (window == NULL) {
		dprintf("Photon: unknown parent for this raw widget!\n");
		return;
	}
	IWindow *wnd = IWindow::Link::Handle2Nmo((const IHandle&)window);
	if (wnd == NULL) {
		dprintf("Photon: Unknown window !\n");
		return;
	}
	if (damage != NULL) {
		if (damage->next != NULL) { damage = damage->next; }
  
		IRect r;
		while (damage != NULL) {
			r.width = damage->rect.lr.x - damage->rect.ul.x;
			r.height = damage->rect.lr.y - damage->rect.ul.y;
			wnd->EventDraw(r);
			wnd->RefreshDisplay(r);
			damage = damage->next;  // Go on the the next tile.
		}
	}
	else {
		IRect r;
		r.width = wnd->Width();
		r.height = wnd->Height();
		wnd->EventDraw(r);
		wnd->RefreshDisplay(r);
	}
}


int nmo_window_callback(PtWidget_t *widget, 
						ApInfo_t *apinfo, 
						PtCallbackInfo_t *cbinfo)
{
	IWindow *wnd = IWindow::Link::Handle2Nmo((const IHandle&)widget);
	if (wnd == NULL) {
		dprintf("Photon: Unknown window !\n");
		return Pt_CONTINUE;
	}
	switch (cbinfo->reason) {

	case Pt_CB_WINDOW:
		{
			PhWindowEvent_t *e = (PhWindowEvent_t*)cbinfo->cbdata;
			switch (e->event_f) {
			case Ph_WM_CLOSE:
				if (wnd->EventQueryDestroy()) {
					PtDestroyWidget(widget);
				}
				break;
			case Ph_WM_MOVE:
				wnd->EventMove();
				break;	
				break;
			default:	
				dprintf("Photon: unknown WINDOW event\n");
				break;
			}
		}
		break;

	case Pt_CB_GOT_FOCUS:
		wnd->EventGotFocus();
		break;

	case Pt_CB_LOST_FOCUS:
		wnd->EventLostFocus();
		break;
		
	case Pt_CB_RESIZE:
		{
			wnd->ResizeDisplay();
			wnd->EventResize();
			PtDamageWidget(widget);
		}
		break;

	case Pt_CB_REALIZED:
		{
			wnd->EventShow();
		}
		break;
		
	case Pt_CB_UNREALIZED:
		wnd->EventHide();
		break;

	case Pt_CB_IS_DESTROYED:
		{
			wnd->EventDestroy();
			ListIterator<IWindow::Link> list(IWindow::Link::handles);
    	    if (((WindowApplication&)Application::App()).mWindowApplicationAbstract ) {
	    	 	IWindow::Link::Drop(*wnd);
			}
   			else {
   			    ((WindowApplication&)Application::App()).mWindowApplicationAbstract = (_PTR)(wnd == list[1].nmo);
	     		IWindow::Link::Drop(*wnd);
				if (((WindowApplication&)Application::App()).mWindowApplicationAbstract ) {
   			    	((WindowApplication&)Application::App()).CloseAllWindows();
   	    	 	}
   			}
        	if (!IWindow::Link::handles.Count()) {
				((WindowApplication&)Application::App()).OnDestroy();
				PtExit(0);
			}
        }
        break;			

	case Pt_CB_RAW:
		{
			PhEvent_t *e = (PhEvent_t*) cbinfo->event;
			_PTR data = PhGetData(e);
			switch (e->type) {
			case Ph_EV_BUT_PRESS:
				{
					PhPointerEvent_t *e = (PhPointerEvent_t*)data;
					PhPoint_t pos;
					PtGetResource(widget, Pt_ARG_POS, &pos, sizeof(pos));
					_DWORD x = e->pos.x ;
					_DWORD y = e->pos.y ;
					if (e->buttons & Ph_BUTTON_SELECT) {
						wnd->EventMouseDown(IMouse::LEFT, x, y);
					}
					if (e->buttons & Ph_BUTTON_MENU) {
						wnd->EventMouseDown(IMouse::RIGHT, x, y);
					}
				}	  
				break;
			case Ph_EV_BUT_RELEASE:
				{
					PhPointerEvent_t *e = (PhPointerEvent_t*)data;
					PhPoint_t pos;
					PtGetResource(widget, Pt_ARG_POS, &pos, sizeof(pos));
					_DWORD x = e->pos.x ;
					_DWORD y = e->pos.y ;
					if (e->buttons & Ph_BUTTON_SELECT) {
						wnd->EventMouseUp(IMouse::LEFT, x, y);
					}
					if (e->buttons & Ph_BUTTON_MENU) {
						wnd->EventMouseUp(IMouse::RIGHT, x, y);
					}
					if (e->buttons & Ph_BUTTON_ADJUST) {
						wnd->EventMouseUp(IMouse::MIDDLE, x, y);
					}
				}	  
				break;
			case Ph_EV_PTR_MOTION_BUTTON:
			case Ph_EV_PTR_MOTION_NOBUTTON:
				{
					PhPointerEvent_t *e = (PhPointerEvent_t*)data;
					PhPoint_t pos;
					PtGetResource(widget, Pt_ARG_POS, &pos, sizeof(pos));
					_DWORD x = e->pos.x ;
					_DWORD y = e->pos.y ;
					if (e->buttons & Ph_BUTTON_SELECT) {
						wnd->EventMouseMove(IMouse::LEFT, x, y);
					}
					if (e->buttons & Ph_BUTTON_MENU) {
						wnd->EventMouseMove(IMouse::RIGHT, x, y);
					}
					if (e->buttons & Ph_BUTTON_ADJUST) {
						wnd->EventMouseMove(IMouse::MIDDLE, x, y);
					}
					else {
						wnd->EventMouseMove(IMouse::NONE, x, y);
					}
				}	  
				break;
			case Ph_EV_KEY:
				break;
			default:
				dprintf("Photon: unknown RAW event\n");
				break;
			}
		break;
		}

	default:
		Debug(dprintf("Photon: unknown event %x\n", cbinfo->reason);)
		break;
	}
	return(Pt_CONTINUE);   
}


//-------------------------------------------------
WindowApplication::WindowApplication(const UStringz &N): Application(N) { }


//-------------------------------------------------
WindowApplication::~WindowApplication() { }


//-------------------------------------------------
void WindowApplication::SetName(const UStringz&) { }


//-------------------------------------------------
_BOOL WindowApplication :: OpenDisplayEnv()
{
	return PtInit(NULL) == 0;
}


//-------------------------------------------------
void WindowApplication :: CloseDisplayEnv()
{
	PtExit(0);
}


//-------------------------------------------------
void WindowApplication :: ProcessNextEvent(IWindow*WINDOW)
{
	ASSERTMSG(FALSE, "Photon port is broken. Missing event dispatcher\n");
	PtMainLoop();
}


//-------------------------------------------------
_BOOL WindowApplication::Create(IWindow::Link &link,
								IWindow::WindowType TYPE,
								const Stringz&)
{
	PtWidget_t 		*parent;

	unsigned short	render_flags = 	0;
	switch (TYPE)
    {
    case IWindow::NORMAL:
		render_flags =  Ph_WM_RENDER_MOVE | Ph_WM_RENDER_COLLAPSE |
						Ph_WM_RENDER_TITLE | Ph_WM_RENDER_BORDER |
						Ph_WM_RENDER_CLOSE;
		break;
	case IWindow::DIALOG:
		render_flags =  Ph_WM_RENDER_MOVE |
						Ph_WM_RENDER_TITLE | Ph_WM_RENDER_BORDER |
						Ph_WM_RENDER_COLLAPSE | Ph_WM_RENDER_CLOSE;
		break;
	case IWindow::POPUP:
		break;
	case IWindow::SIZEABLE:
		render_flags =  Ph_WM_RENDER_MOVE | Ph_WM_RENDER_COLLAPSE |
						Ph_WM_RENDER_TITLE | Ph_WM_RENDER_BORDER |
						Ph_WM_RENDER_MIN | Ph_WM_RENDER_MAX |
						Ph_WM_RENDER_RESIZE | Ph_WM_RENDER_CLOSE;
		break;
	default:
		break;
	};  

	PtArg_t 		argt[5];
	PhPoint_t 		pos ={ 10, 10 };
	Stringz 		name = Name();

	_DWORD n = 0;
	PtSetArg(&argt[n++], Pt_ARG_POS, &pos, 0);
	PtSetArg(&argt[n++], Pt_ARG_WINDOW_RENDER_FLAGS, render_flags, 0);
	PtSetArg(&argt[n++], Pt_ARG_WINDOW_NOTIFY_FLAGS, 
			 //Ph_WM_RESIZE | Ph_WM_CLOSE | Ph_WM_MOVE, 
			 Ph_WM_CLOSE | Ph_WM_MOVE, 
			 0);
	PtSetArg(&argt[n++], Pt_ARG_WINDOW_MANAGED_FLAGS, 
			 Ph_WM_CLOSE | Ph_WM_MOVE, 
			 0);
	PtSetArg(&argt[n++], Pt_ARG_WINDOW_TITLE, (_CHAR*)name, 0);


    // The first created window is the parent of all
    // others windows
    if (IWindow::Link::handles.Count() > 1) {
		ListIterator<IWindow::Link> lst(IWindow::Link::handles);
		parent = IHANDLE(lst[1].handle);
	}
    else {	
		parent = Pt_NO_PARENT;
	}

	PtWidget_t *wnd = PtCreateWidget(PtWindow, parent, n, argt);
	if (!wnd) {
		dprintf("PtCreateWidget failed\n");
		return FALSE;
	}
	link.handle = (_PTR)wnd;

	PhRect_t arect = { 0, 0, 0, 0 };
	pos.x = 0;
	pos.y = 0;
	n = 0;
	PtSetArg(&argt[n++], Pt_ARG_ANCHOR_FLAGS,
			     	Pt_LEFT_ANCHORED_LEFT |
					Pt_RIGHT_ANCHORED_RIGHT |
					Pt_BOTTOM_ANCHORED_BOTTOM |
				    Pt_TOP_ANCHORED_TOP,
					Pt_LEFT_IS_ANCHORED |
					Pt_RIGHT_IS_ANCHORED |
					Pt_BOTTOM_IS_ANCHORED |
			     	Pt_TOP_IS_ANCHORED);
	PtSetArg(&argt[n++], Pt_ARG_ANCHOR_OFFSETS, &arect, 0);
	PtSetArg(&argt[n++], Pt_ARG_POS, &pos, 0);
	PtSetArg(&argt[n++], Pt_ARG_RAW_DRAW_F, &nmo_raw_callback, 0);
	PtWidget_t *raw = PtCreateWidget(PtRaw, wnd, n, argt);
	link.nmo->raw = (_PTR)raw;

    IDisplayPhoton *DISP = new IDisplayPhoton(IDisplay::DISPLAY_DOUBLE, raw);
    if (!DISP) {
        dprintf("WindowApplication: Creating display\n");
        return FALSE;
    }
    link.nmo->display = (IDisplay*) DISP;

	PtAddCallback(wnd, Pt_CB_WINDOW, nmo_window_callback, NULL);
	PtAddCallback(wnd, Pt_CB_RESIZE, nmo_window_callback, NULL);
	PtAddCallback(wnd, Pt_CB_GOT_FOCUS, nmo_window_callback, NULL);
	PtAddCallback(wnd, Pt_CB_LOST_FOCUS, nmo_window_callback, NULL);
	PtAddCallback(wnd, Pt_CB_REALIZED, nmo_window_callback, NULL);
	PtAddCallback(wnd, Pt_CB_UNREALIZED, nmo_window_callback, NULL);
	PtAddCallback(wnd, Pt_CB_IS_DESTROYED, nmo_window_callback, NULL);
	PtAddEventHandler(wnd, Ph_EV_KEY, nmo_window_callback, NULL);
	PtAddEventHandler(wnd, Ph_EV_PTR_MOTION_NOBUTTON, nmo_window_callback, NULL);
	PtAddEventHandler(wnd, Ph_EV_PTR_MOTION_BUTTON, nmo_window_callback, NULL);
	PtAddEventHandler(wnd, Ph_EV_BUT_PRESS, nmo_window_callback, NULL);
	PtAddEventHandler(wnd, Ph_EV_BUT_RELEASE, nmo_window_callback, NULL);
	
	return TRUE;
}

#endif
