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

$Id: WindowApplicationBeOS.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifdef __BEOS__
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <nmo/WindowApplication.hpp>
#include "interface/IDisplayBeOS.hpp"
#include "interface/BeWindow.hpp"
using namespace NMO;

typedef struct 
{
	BApplication *bapp;
} AbstractBeWinApp;

#define __m(x) ((AbstractBeWinApp*)(this->abstract))->x

//-------------------------------------------------
WindowApplication::WindowApplication(const UStringz &N): Application(N) 
{ 
	abstract = (_PTR) new AbstractBeWinApp;
	__m(bapp) = new BApplication("application/x-vnd.nmo-sdk");
}


//-------------------------------------------------
WindowApplication::~WindowApplication() 
{ 
	delete __m(bapp);
	delete (AbstractBeWinApp*)abstract;
}


//-------------------------------------------------
void WindowApplication::SetName(const UStringz&) { }


//-------------------------------------------------
void WindowApplication::Run()
{
	OnCreate();
	__m(bapp)->Run();
	OnDestroy();
}


//-------------------------------------------------
void WindowApplication::Terminate()
{
	OnDestroy();
	__m(bapp)->Lock();
	__m(bapp)->Quit();
}


//-------------------------------------------------
_BOOL WindowApplication::Create(IWindow::Link &link,
								IWindow::WindowType TYPE,
								const Stringz&)
{
    // The first created window is the parent of all
    // others windows
    /*if (IWindow::Link::handles.Count() > 1) {
		ListIterator<IWindow::Link> lst(IWindow::Link::handles);
	}
    else {	
	}*/

	window_look wlook;
	switch (TYPE)
    {
    case IWindow::NORMAL:
		wlook = B_TITLED_WINDOW_LOOK;   	
		break;
	case IWindow::DIALOG:
		wlook = B_FLOATING_WINDOW_LOOK;   	
		break;
	case IWindow::POPUP:
		wlook = B_BORDERED_WINDOW_LOOK;
		break;
	case IWindow::SIZEABLE:
		wlook = B_TITLED_WINDOW_LOOK;   	
		break;
	default:
		break;
	};  

	BRect aRect(50, 50, 10, 10);
	Stringz title = Name();
	link.handle = new BeWindow(link.nmo, aRect, title, wlook);
	if (!link.handle) {
        dprintf("WindowApplication: Creating window\n");
		return FALSE;
	}

	/*switch (TYPE)
    {
    case IWindow::NORMAL:
    	link.handle->SetType(B_NOT_RESIZABLE | B_NOT_ZOOMABLE);
		break;
	case IWindow::DIALOG:
    	link.handle->SetType(B_NOT_RESIZABLE | B_NOT_CLOSABLE | B_NOT_ZOOMABLE);
		break;
	case IWindow::POPUP:
    	link.handle->SetType(B_NOT_RESIZABLE | B_NOT_CLOSABLE);
		break;
	case IWindow::SIZEABLE:
		break;
	default:
		break;
	};  */

    IDisplayBeOS *DISP = new IDisplayBeOS(IDisplay::DISPLAY_DOUBLE);
    if (!DISP) {
        dprintf("WindowApplication: Creating display\n");
        return FALSE;
    }
    link.nmo->display = (IDisplay*) DISP;

	return TRUE;
}

#endif
