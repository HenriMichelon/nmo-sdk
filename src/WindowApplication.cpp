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

$Id: WindowApplication.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <nmo/WindowApplication.hpp>
using namespace NMO;


//-------------------------------------------------
_BOOL WindowApplication :: Add(	IWindow&WND,
								IWindow::WindowType TYPE,
								const Stringz&OPTION)
{
	// Cannot add a window two times
	ListIterator<IWindow::Link> lst(IWindow::Link::handles);
	while (!lst.End()) {
		if (lst.Next().nmo == &WND) {
			dprintf("WindowApplication:Add: multiple attachement of a window\n");
			return FALSE;
		}
	}

	// drop any existing display
	if (WND.display != NULL) {
		delete WND.display;
		WND.display = NULL;
	}

	// Create a new handle to GWindow link &
	// add it to the application links list
	IWindow::Link *link = new IWindow::Link(&WND, FALSE);
	// Call architecture dependent window creation method
	if (Create(*link, TYPE, OPTION)) {
		IWindow::Link::handles.Add(link);
		ASSERT(link->nmo->display);
	    if (link->nmo->display->CreateBuffers()) {
			// Set local keyboard mapping
  			link->nmo->keyb.SetMap();
			// IWindow known their system depent own handle
			link->nmo->handle = link->handle;
			link->nmo->SetTitle(Name());
			link->nmo->EventCreate();
			return TRUE;
		}
		else {
			// Error creating display buffers
			delete link->nmo->display;
			link->nmo->display = NULL;
			dprintf("WindowApplication: initializing display\n");
		}
	}
	// Error creating window
	delete link;
	return FALSE;
}


//-------------------------------------------------
void WindowApplication :: CloseAllWindows()
{
	ListIterator<IWindow::Link> list(IWindow::Link::handles);
	while (!list.End()) {
		list.Next().nmo->Close();
	}
}


//-------------------------------------------------
void WindowApplication :: Run()
{
	mQuit = FALSE;
	if (OpenDisplayEnv()) {
		try {
			OnCreate();
			while (!mQuit) {
				ProcessNextEvent(NULL);
			}
	        CloseAllWindows();
			OnDestroy();
		}
		catch (NMOException e)
		{
			e.Display();
		}
		CloseDisplayEnv();
	}
}


//-------------------------------------------------
void WindowApplication :: Terminate()
{
	mQuit = TRUE;
}


//-------------------------------------------------
_BOOL WindowApplication :: Terminated() const
{
	return mQuit;
}
