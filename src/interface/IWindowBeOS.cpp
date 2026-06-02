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

$Id: IWindowBeOS.cpp,v 1.2 2002/11/29 09:32:25 hmichelon Exp $
-------------------------------------------------------------------*/
#ifdef __BEOS__
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include "IDisplayBeOS.hpp"
using namespace NMO;

//------------------------------------------------------------
IWindow::IWindow(): display(NULL), handle(NULL) {}


//------------------------------------------------------------
IWindow::~IWindow()
{
	if (display) delete display;
	Link::Drop(*this);
}


//------------------------------------------------------------
void IWindow::SetTitle(const UStringz&TEXT)
{
	Stringz title = TEXT;
	handle->SetTitle(title);
}


//------------------------------------------------------------
UStringz IWindow::Title() const
{
	return handle->Title();
}


//------------------------------------------------------------
_BOOL IWindow::Visible() const
{
	return (!handle->IsHidden());
}


//------------------------------------------------------------
void IWindow::Show(_BOOL S)
{
	if (S) {
		handle->Show();
	}
	else {
		handle->Hide();
	}
}


//------------------------------------------------------------
void IWindow::Close()
{
	handle->Quit();
}


//------------------------------------------------------------
_LONG IWindow::Top() const
{
	return (_LONG)handle->Frame().top;
}


//------------------------------------------------------------
_LONG IWindow::Left() const
{
	return (_LONG)handle->Frame().left;
}


//------------------------------------------------------------
void IWindow::SetPos(_LONG L, _LONG T)
{
	handle->MoveTo(float(L), float(T));
}


//------------------------------------------------------------
_DWORD IWindow::Width() const
{
	return (_DWORD)ABS(handle->Bounds().right - handle->Bounds().left);
}


//------------------------------------------------------------
_DWORD IWindow::Height() const
{
	return (_DWORD)ABS(handle->Bounds().bottom - handle->Bounds().top);
}


//------------------------------------------------------------
void IWindow::SetSize(_DWORD W, _DWORD H)
{
	handle->ResizeTo(float(W), float(H));
}

	
//------------------------------------------------------------
IRect IWindow::Rect() const
{
	IRect res((_LONG)handle->Frame().left,
				(_LONG)handle->Frame().top,
				(_DWORD)ABS(handle->Bounds().right - handle->Bounds().left),
				(_DWORD)ABS(handle->Bounds().bottom - handle->Bounds().top));
	return res;
}


//------------------------------------------------------------
_BOOL IWindow::HasFocus() const
{
	return (handle->IsFront());
}


//------------------------------------------------------------
void IWindow::GiveFocus()
{
}


//------------------------------------------------------------
void IWindow::RefreshDisplay(const IRect&R)
{
	ASSERT(display);
	display->SetClip(R.left, R.top, R.width, R.height);
	display->Refresh(R);
}


//------------------------------------------------------------
void IWindow::ResizeDisplay()
{
	ASSERT(display);
	IDisplayBeOS *disp = (IDisplayBeOS*)display;
	disp->DestroyBuffer();
	disp->CreateBuffer(handle);
}


//------------------------------------------------------------
void IWindow :: SetBgColor (IRGBColor &COLOR)
{
}


#endif
