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

$Id: IWindow.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;

NMO::List<IWindow::Link> IWindow::Link::handles;


//------------------------------------------------------------
void IWindow::SetRect(_LONG L, _LONG T, _DWORD W, _DWORD H)
{
	SetPos(L, T);
	SetSize(W, H);
}


//------------------------------------------------------------
void IWindow::SetRect(const IRect&R)
{
	SetRect(R.left, R.top, R.width, R.height);
}


//------------------------------------------------------------
void IWindow::ScreenCenter()
{
	/*Debug(dprintf("IWindow::ScreenCenter: %dx%d - %dx%d\n",
					IScreen::Width(),
					IScreen::Height(),
					Width(),
					Height());)*/
	SetPos((IScreen::Width() - Width())/2,
		(IScreen::Height() - Height())/2);
}


//------------------------------------------------------------
IWindow* IWindow::Link::Handle2Nmo(const IHandle &W)
{
	ListIterator<IWindow::Link> list(handles);
	while (!list.End()) {
		IWindow::Link &link = list.Next();
		if (link.handle == W) return link.nmo;
	}
	return NULL;
}


//------------------------------------------------------------
IHandle IWindow::Link::Nmo2Handle(const IWindow&O)
{
	const IWindow *o = &O;
	ListIterator<IWindow::Link> list(handles);
	while (!list.End()) {
		IWindow::Link &link = list.Next();
		if (link.nmo == o) return link.handle;
	}
	Debug(throw NMOException("IWindow::Link: unknow IWindow"););
	return (IHandle)NULL;
}

//------------------------------------------------------------
void IWindow::Link::Drop(const IWindow&O)
{
	const IWindow *o = &O;
	ListIterator<IWindow::Link> list(handles);
	while (!list.End()) {
		IWindow::Link &link = list.Next();
		if (link.nmo == o) {
			handles.Drop(list.Index() - 1);
		}
	}
}


//------------------------------------------------------------
void IWindow::RefreshDisplay(const IRect&R)
{
	ASSERT(display);
	if (Visible()) {
		display->SetClip(R.left, R.top, R.width, R.height);
		display->Refresh(R);
	}
}


//------------------------------------------------------------
void IWindow::ResizeDisplay()
{
	ASSERT(display);
	if ((handle != NULL) &&
		( (display->Width() != Width()) ||
		(display->Height() != Height()))) {
		display->DestroyBuffers();
		display->CreateBuffers();
	}
}


