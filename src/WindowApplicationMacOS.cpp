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

$Id: WindowApplicationMacOS.cpp,v 1.2 2002/11/29 13:27:52 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <nmo/WindowApplication.hpp>
#include <nmo/Thread.hpp>
using namespace NMO;
#include "interface/IDisplayMacOS.hpp"

class AbstractWinappMacOS {
public:
	/*enum {
		MENU_FILE	= 2000
	};
	enum {
		ITEM_QUIT	= 1
	};
	MenuHandle		mnuFile;	// the File menu*/
	static _BOOL	queryquit;
} ;

_BOOL AbstractWinappMacOS::queryquit = FALSE;

#define __m(x) ((AbstractWinappMacOS*)((WindowApplication*)winapp)->mWindowApplicationAbstract)->x

static pascal OSStatus
nmo_WindowEventHandlerUPP(EventHandlerCallRef, 
							EventRef event, 
							void *winapp)
{
	OSStatus	result = noErr;
	WindowRef	window;
	Rect		bounds;
	UInt32		kind = GetEventKind(event);

 	GetEventParameter(event, kEventParamDirectObject, typeWindowRef, 
 						  NULL, sizeof(window), NULL, &window);
	IWindow *wnd = IWindow::Link::Handle2Nmo(window);
	if (!wnd) {
		EventRecord eventrec;
		if (ConvertEventRefToEventRecord(event, &eventrec)) {
			FindWindow(eventrec.where, &window);
			wnd = IWindow::Link::Handle2Nmo(window);
		}
	}
	if (!wnd) {
		window = FrontWindow();
		wnd = IWindow::Link::Handle2Nmo(window);
	}
	if (!wnd) { 
		dprintf("Received event for unknown window\n");
		return eventNotHandledErr; 
	}
	switch (GetEventClass(event))
	{
	case kEventClassWindow:
	    switch (kind) {
	    case kEventWindowResizeCompleted:
			wnd->ResizeDisplay();
			wnd->EventResize();
	        InvalWindowRect(window, GetWindowPortBounds(window, &bounds));
        	break;
	    case kEventWindowDragCompleted:
	    	wnd->EventMove();
	    	break;
    	case kEventWindowUpdate:
    	case kEventWindowDrawContent:
	    	if (wnd->Visible()) {
				IRect r;
				GetWindowPortBounds(window, &bounds);
				r.left = bounds.left;
				r.top = bounds.top;
				r.width = bounds.right - bounds.left;
				r.height = bounds.bottom - bounds.top;
				wnd->EventDraw(r);
				wnd->RefreshDisplay(r);
		    }
		    break;
	    case kEventWindowClose:
    		if (wnd->EventQueryDestroy()) {
	    	    DisposeWindow(window);
		    }
    	    break;
	    case kEventWindowActivated:
    		wnd->EventGotFocus();
    		break;
	    case kEventWindowDeactivated:
    		wnd->EventLostFocus();
    		break;
	    case kEventWindowShown:
	    	wnd->EventShow();
	    	break;
	    case kEventWindowHidden:
	    	wnd->EventHide();
	    	break;
	    case kEventWindowDispose:
			{
				wnd->EventDestroy();
				ListIterator<IWindow::Link> list(IWindow::Link::handles);
				if (!__m(queryquit)) {
					__m(queryquit) = (window == list[1].handle);
					IWindow::Link::Drop(*wnd);
					if (__m(queryquit)) {
						((WindowApplication*)winapp)->CloseAllWindows();
					}
				}
				else {
					IWindow::Link::Drop(*wnd);
				}
				if (!IWindow::Link::handles.Count()) {
					Application::App().Terminate();
					QuitApplicationEventLoop();
				}
			}
			break;
	    default:
		    result = eventNotHandledErr;
	    	break;
	    }
	    break;
	case kEventClassMouse:
		{
			Point point;
			UInt16 msbtn;
			//UInt32 modifier;
			IMouse::Button btn;
			GetEventParameter(event, kEventParamMouseLocation, typeQDPoint, 
							NULL, sizeof(point), NULL, &point);						
			if (FindWindow(point, &window) != inContent) {
				return eventNotHandledErr;
			}
			GetEventParameter(event, kEventParamMouseButton, typeMouseButton, 
							NULL, sizeof(msbtn), NULL, &msbtn);
			switch (msbtn) {
			case kEventMouseButtonPrimary:
					btn = IMouse::LEFT;
				break;
			case kEventMouseButtonSecondary:
				btn = IMouse::MIDDLE;
				break;
			case kEventMouseButtonTertiary:
				btn = IMouse::RIGHT;
				break;
			}
			GetWindowBounds(window, kWindowContentRgn, &bounds);
			_LONG x = point.h - bounds.left;
			_LONG y = point.v - bounds.top;
			switch (kind) {	    
		    case kEventMouseDown:
		    	if (window != FrontWindow()) {	SelectWindow(window); }
		    	wnd->EventMouseDown(btn, x, y);
		    	break;
		    case kEventMouseUp:
		    	wnd->EventMouseUp(btn, x, y);
		    	break;
		    case kEventMouseMoved:
		    case kEventMouseDragged:
		    	wnd->EventMouseMove(btn, x, y);
	    		break;
		    default:
			    result = eventNotHandledErr;
	    		break;
	    	}
	    }
	    break;
	case kEventClassKeyboard:
		{
			if (window != FrontWindow()) {	SelectWindow(window); }
			char code;
			GetEventParameter(event, kEventParamKeyMacCharCodes, typeChar, 
							NULL, sizeof(code), NULL, &code);
	    	wnd->Keyb().SetModifiers(event);
			switch (kind) {	    
		    case kEventRawKeyDown:
		    case kEventRawKeyRepeat:
		    	wnd->Keyb().SetKey((IKeyb::Key)code);
		    	wnd->EventKeybDown((IKeyb::Key)code);
		    	break;
		    case kEventRawKeyUp:
		    	wnd->Keyb().UnsetKey((IKeyb::Key)code);
		    	wnd->EventKeybUp((IKeyb::Key)code);
		    	break;
		    default:
			    result = eventNotHandledErr;
		    	break;
		    }
		}
	    break;
	case kEventClassMenu:
		//QuitApplicationEventLoop();
		break;
    default:
	    result = eventNotHandledErr;
    	break;
    }
    return result;
}



//-------------------------------------------------
WindowApplication::WindowApplication(const UStringz &N): Application(N)
{
	mWindowApplicationAbstract = new AbstractWinappMacOS;
}


//-------------------------------------------------
WindowApplication::~WindowApplication()
{
	delete (AbstractWinappMacOS*)mWindowApplicationAbstract;
}


//-------------------------------------------------
void WindowApplication::SetName(const UStringz&NAME)
{
	NMOException::SetTitle(NAME);
}


//-------------------------------------------------
_BOOL WindowApplication :: OpenDisplayEnv()
{
	GDHandle device = GetGDevice ();
	IScreen::SetSize((*device)->gdRect.right, 
					 (*device)->gdRect.bottom);

	InitCursor();
	return TRUE;
}


//-------------------------------------------------
void WindowApplication :: CloseDisplayEnv()
{
	IColorSpace::colorSpace.Palette().Clear();
}


//-------------------------------------------------
void WindowApplication :: ProcessNextEvent(IWindow*WINDOW)
{
	// Technical Q&A: QA1061, http://developer.apple.com/qa/qa2001/qa1061.html
	EventRef       theEvent;
	EventTimeout   timeToWaitForEvent;
	EventTargetRef theTarget = GetEventDispatcherTarget();
	if (ThThread::gNumberOfThreads == 0) {
		timeToWaitForEvent = kEventDurationForever;
	}
	else {
		timeToWaitForEvent = kEventDurationNoWait;
	}
	OSErr err = ReceiveNextEvent(0, 
							NULL, 
							timeToWaitForEvent,
							true, 
							&theEvent);
	if (err == noErr) {
		SendEventToEventTarget(theEvent, theTarget);
		ReleaseEvent(theEvent);
	}
	if (ThThread::gNumberOfThreads > 0) { YieldToAnyThread(); }
}


//-------------------------------------------------
_BOOL WindowApplication::Create(IWindow::Link &link,
								IWindow::WindowType TYPE,
								const Stringz&)
{
	Rect    rect;
	SetRect(&rect,50,50,60,60);

	Str255 name;
	Stringz namez;
	CopyCStringToPascal(namez, name);

	WindowAttributes attribs =  kWindowStandardHandlerAttribute |
								kWindowCollapseBoxAttribute |
								0; // kWindowNoBufferingAttribute For MacOS X double buffered windows.
	switch (TYPE)
	{
	case IWindow::NORMAL:
		attribs |= kWindowCloseBoxAttribute;
		break;
	case IWindow::SIZEABLE:
		attribs |= kWindowStandardDocumentAttributes;
		break;
	case IWindow::DIALOG:
	case IWindow::POPUP:
		break;
	};

	OSErr err;
	/*if (TYPE == IWindow::POPUP) {
		WindowDefSpec specs;
		specs.defType = kWindowDefProcPtr;
		specs.u.defProc = NULL;
		err =  CreateCustomWindow(&specs,
								kDocumentWindowClass,
								attribs | kWindowStandardHandlerAttribute,
								&rect,
								&link.handle);								
	}
	else {*/
		err = CreateNewWindow(kDocumentWindowClass,
								attribs,
								&rect,
								(OpaqueWindowPtr**)&link.handle);
	//}
	if (err != noErr) {
		dprintf("WindowApplication: CreateNewWindow failed\n");
		return FALSE;
	}							

	// Install a Carbon event handler for this window
	EventHandlerRef ref;
	EventTypeSpec list[] = {
						   { kEventClassWindow, kEventWindowClose },
						   { kEventClassWindow, kEventWindowDrawContent },
						   //{ kEventClassWindow, kEventWindowUpdate },
						   { kEventClassWindow, kEventWindowResizeCompleted },
						   { kEventClassWindow, kEventWindowDragCompleted },
						   //{ kEventClassWindow, kEventWindowBoundsChanging },
						   { kEventClassWindow, kEventWindowActivated },
						   { kEventClassWindow, kEventWindowDeactivated },
						   { kEventClassWindow, kEventWindowShown },
						   { kEventClassWindow, kEventWindowHidden },
						   { kEventClassWindow, kEventWindowDispose },
						   { kEventClassMouse, kEventMouseDown },
						   { kEventClassMouse, kEventMouseUp },
						   { kEventClassMouse, kEventMouseMoved },
						   { kEventClassMouse, kEventMouseDragged },
						   { kEventClassKeyboard, kEventRawKeyDown },
						   { kEventClassKeyboard, kEventRawKeyRepeat },
						   { kEventClassKeyboard, kEventRawKeyUp },
						  // { kEventClassMenu, kEventMenuEndTracking }
						   };
							   
	InstallWindowEventHandler(IHANDLE(link.handle),
							  NewEventHandlerUPP(nmo_WindowEventHandlerUPP),
							  GetEventTypeCount(list),
							  list,
							  this,
							  &ref);

	IDisplayMacOS *DISP = new IDisplayMacOS(IDisplay::DISPLAY_SIMPLE, IHANDLE(link.handle));
	link.nmo->display = DISP;
	return TRUE;
}
#endif