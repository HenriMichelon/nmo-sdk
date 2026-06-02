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

$Id: IWindowPhoton.cpp,v 1.2 2002/11/29 09:32:25 hmichelon Exp $
-------------------------------------------------------------------*/
#ifdef __QNX__
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <Pt.h>
#include "IDisplayPhoton.hpp"
using namespace NMO;


#define WINHANDLE ((PtWidget_t*)(this->handle))
#define RAWHANDLE ((PtWidget_t*)(this->raw))

//------------------------------------------------------------
IWindow::IWindow()
{
	display = NULL;
	handle = NULL;
}


//------------------------------------------------------------
IWindow::~IWindow()
{
	if (display) { delete display; }
    Link::Drop(*this);
}


//------------------------------------------------------------
void IWindow::SetTitle(const UStringz&TEXT)
{
	Stringz title = TEXT;
	PtSetResource(WINHANDLE, Pt_ARG_TITLE, (_CHAR*)title, title.Len());
}


//------------------------------------------------------------
UStringz IWindow::Title() const
{
	_CHAR title[2000];
	PtGetResource(WINHANDLE, Pt_ARG_TITLE, title, 2000);
	return title;
}


//------------------------------------------------------------
_BOOL IWindow::Visible() const
{
	unsigned short state;
	PtGetResource(WINHANDLE, Pt_ARG_WINDOW_STATE, &state, sizeof(state));
	return FALSE;
}


//------------------------------------------------------------
void IWindow::Show(_BOOL S)
{
	if (S) {
		PtRealizeWidget(WINHANDLE);
	}
	else {
		PtUnrealizeWidget(WINHANDLE);
	}
}


//------------------------------------------------------------
void IWindow::Close()
{
	PtDestroyWidget(WINHANDLE);
}



//------------------------------------------------------------
_LONG IWindow::Top() const
{
	PhPoint_t *pos;
	PtGetResource(WINHANDLE, Pt_ARG_POS, &pos, 0);
	return pos->y;
}


//------------------------------------------------------------
_LONG IWindow::Left() const
{
	PhPoint_t *pos;
	PtGetResource(WINHANDLE, Pt_ARG_POS, &pos, 0);
	return pos->x;
}


//------------------------------------------------------------
void IWindow::SetPos(_LONG L, _LONG T)
{
	PhPoint_t pos = { L, T };
	PtSetResource(WINHANDLE, Pt_ARG_POS, &pos, sizeof(pos));
}


//------------------------------------------------------------
_DWORD IWindow::Width() const
{
	unsigned short *width;
	PtGetResource(WINHANDLE, Pt_ARG_WIDTH, &width, 0);
	return *width;
}


//------------------------------------------------------------
_DWORD IWindow::Height() const
{
	unsigned short *height;
	PtGetResource(WINHANDLE, Pt_ARG_HEIGHT, &height, 0);
	return *height;
}


//------------------------------------------------------------
void IWindow::SetSize(_DWORD W, _DWORD H)
{
	PhArea_t *warea, *rarea;
	PtGetResource(WINHANDLE, Pt_ARG_AREA, &warea, 0);
	PtGetResource(RAWHANDLE, Pt_ARG_AREA, &rarea, 0);

	PtSetResource(WINHANDLE, Pt_ARG_WIDTH, W + warea->size.w - rarea->size.w, 0);
	PtSetResource(WINHANDLE, Pt_ARG_HEIGHT, H + warea->size.h - rarea->size.h, 0);
}

	
//------------------------------------------------------------
IRect IWindow::Rect() const
{
	IRect res;
	PhArea_t *area;
	PtGetResource(WINHANDLE, Pt_ARG_AREA, &area, 0);
	res.left = area->pos.x;
	res.top = area->pos.y;
	res.width = area->size.w;
	res.height = area->size.h;
	return res;
}


//------------------------------------------------------------
_BOOL IWindow::HasFocus() const
{
	return  PtIsFocused(WINHANDLE) == 2;
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
	IDisplayPhoton *disp = (IDisplayPhoton*)display;
	disp->RealRefresh(R);
	//PtDamageWidget(RAWHANDLE);
}


//------------------------------------------------------------
void IWindow::ResizeDisplay()
{
	ASSERT(display);
	/*PhRect_t rect;
	PtCalcCanvas (WINHANDLE, &rect);
	PtSetResource(RAWHANDLE, Pt_ARG_WIDTH, rect.lr.x - rect.ul.x + 1, 0);
	PtSetResource(RAWHANDLE, Pt_ARG_HEIGHT, rect.lr.y - rect.ul.y + 1, 0);*/
	IDisplayPhoton *disp = (IDisplayPhoton*)display;
	disp->DestroyBuffers();
	disp->CreateBuffers();
}


//------------------------------------------------------------
void IWindow :: SetBgColor (IRGBColor &COLOR)
{
}


#endif
