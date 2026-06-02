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

	(c) 1998-2001 Henri Michelon

$Id: IPixmap.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/interface/IRect.hpp>
#include <nmo/interface/IRGBColor.hpp>
#include <nmo/interface/IPalette.hpp>
#include <nmo/interface/IDisplay.hpp>
#include <nmo/interface/IColorSpace.hpp>
#include <nmo/interface/IPixmap.hpp>
using namespace NMO;


//----------------------------------------------------------------------------
IPixmap :: IPixmap()
{
	count = 0;
	frames = NULL;
	clone = FALSE;
	remaped = FALSE;
}


//----------------------------------------------------------------------------
void IPixmap::SetTransColor(const IRGBColor&COL)
{
	if (frames)	{
		IFrame *Tmp = frames;
		for (_DWORD i=0; i<count; i++) {
			Tmp->trans = TRUE;
			Tmp->transcolor = COL;
			Tmp = Tmp->next;
		}
	}
}


//----------------------------------------------------------------------------
const IRGBColor& IPixmap::TransColor() const
{
	ASSERT(frames);
	return frames->transcolor;
}


//----------------------------------------------------------------------------
IPixmap* IPixmap::Clone() const
{
	VALIDATETHIS;
	//dp("clone IPixmap %x\n", this);
	IPixmap *tmp = new IPixmap;
	if (frames) {
		IFrame *frame = frames;
		IFrame *ftmp;
		for (_WORD i=0; i<count; i++) {
			ftmp = tmp->AddFrame(frame->sleepTimeTotal);
			ftmp->width = frame->width;
			ftmp->height = frame->height;
			ftmp->bpp = frame->bpp;
			ftmp->trans = frame->trans;
			ftmp->transcolor = frame->transcolor;
			ftmp->pic = frame->pic;
			ftmp->pal = frame->pal->Clone();
			frame = frame->next;
		}
	}
	tmp->clone = TRUE;
	tmp->remaped = remaped;
	return tmp;
}


//----------------------------------------------------------------------------
IPixmap* IPixmap::Copy() const
{
	VALIDATETHIS;
	IPixmap *tmp = new IPixmap;
	if (frames) {
		IFrame *frame = frames;
		IFrame *ftmp;
		_DWORD size;
		for (_WORD i=0; i<count; i++) {
			ftmp = tmp->AddFrame(frame->sleepTimeTotal);
			ftmp->width = frame->width;
			ftmp->height = frame->height;
			ftmp->bpp = frame->bpp;
			ftmp->trans = frame->trans;
			ftmp->transcolor = frame->transcolor;
			if (frame->pic) {
				ASSERT(ftmp->width);
				ASSERT(ftmp->height);
				ASSERT(ftmp->bpp);
				size = ftmp->width*ftmp->height*ftmp->bpp;
				ftmp->pic = new _BYTE[size];
				Memory::Copy(ftmp->pic, frame->pic, size);
			}
			if (frame->pal)
				ftmp->pal = frame->pal->Copy();
			frame = frame->next;
		}
	}
	tmp->clone = FALSE;
	tmp->remaped = remaped;
	return tmp;
}


//----------------------------------------------------------------------------
IPixmap :: ~IPixmap()
{
	VALIDATETHIS;
	if (frames) {
		IFrame *Tmp = frames;
		for (_WORD i=0; i<count; i++) {
			//dp("delete %x %x %x\n", Tmp, Tmp->pic, Tmp->pal);
			if (!clone)
				if (Tmp->pic) delete []Tmp->pic;
			frames = Tmp->next;
			if (Tmp->pal) 
				delete Tmp->pal;
			delete Tmp;
			Tmp = frames;
		}
		frames = NULL;
	}
	count = 0;
}



//----------------------------------------------------------------------------
IPixmap::IFrame* IPixmap :: AddFrame(_WORD sleeptime)
{
	VALIDATETHIS;
	ASSERT(!clone);

	IFrame *New = new IFrame;
	ASSERT(New);

	New->sleepTimeTotal = sleeptime;
	New->sleepTimeRemain = New->sleepTimeTotal;
	New->width = 0;
	New->height = 0;
	New->bpp = 0;
	New->transcolor = IRGBColor(0,0,0);
	New->trans = TRUE;
	New->pal = NULL;
	if (!count)
		frames = New;
	else
	{
		ASSERT(frames);
		New->next = frames->next;
	}
	frames->next = New;
	frames = New;
	count ++;
	//dp("Add frame %d %x\n", count, New);
	return New;
}


//----------------------------------------------------------------------------
IPixmap::IFrame* IPixmap :: Frame(_WORD idx) const
{
	VALIDATETHIS;
	ASSERT(idx<count);

	IFrame *Tmp = frames;
	for (_LONG i=0; i<idx; i++) {
		ASSERT(Tmp);
		Tmp = Tmp->next;
	}
	//Dg(dp("Get Frame %d\n", idx);)
	return Tmp;
}


//----------------------------------------------------------------------------
_WORD IPixmap :: FramesCount() const
{
	VALIDATETHIS;
	return count;
}


//----------------------------------------------------------------------------
void IPixmap :: Remap()
{
	VALIDATETHIS;

	if (remaped) return;
	IFrame *Tmp = frames;
	void *Ptr;
	for (_WORD i=0; i<count; i++)
	{
		ASSERT(Tmp);
		//ASSERTMSG(Tmp->bpp == 1, "IPixmap: not a 1 bpp pixmap");
		Ptr = (void*) Tmp->pic;
		ASSERT(Ptr);
		Tmp->bpp = (_BYTE)IColorSpace::colorSpace.Remap((void*&)Ptr, 
												Tmp->pal, 
												Tmp->width * Tmp->height, 
												Tmp->bpp);
		ASSERT(Ptr);
		Tmp->pic = (_BYTE*) Ptr;
		Tmp = Tmp->next;
	}
	remaped = TRUE;
}


//----------------------------------------------------------------------------
void IPixmap :: DrawNoTransNoClip(IDisplay& display, _LONG X, _LONG Y)
{
	ASSERT(frames);

	display.DrawNoTransNoClip(frames->pic, X, Y, frames->width, frames->height);
	if (frames->sleepTimeRemain)
		frames->sleepTimeRemain--;
	else
	{
		frames->sleepTimeRemain = frames->sleepTimeTotal;
		frames = frames->next;
		ASSERT(frames);
	}
}


//----------------------------------------------------------------------------
void IPixmap :: DrawTransNoClip(IDisplay& display, _LONG X, _LONG Y)
{
	ASSERT(frames);

	IRGBColor oldtranscolor = display.TransColor();
	display.SetTransColor(frames->transcolor);
	display.DrawTransNoClip(frames->pic, X, Y, frames->width, frames->height);
	display.SetTransColor(oldtranscolor);
	if (frames->sleepTimeRemain)
		frames->sleepTimeRemain--;
	else
	{
		frames->sleepTimeRemain = frames->sleepTimeTotal;
		frames = frames->next;
		ASSERT(frames);
	}
}


//----------------------------------------------------------------------------
void IPixmap :: DrawNoTransClip(IDisplay& display, _LONG X, _LONG Y)
{
	ASSERT(frames);

	display.DrawNoTransClip(frames->pic, X, Y, frames->width, frames->height);
	if (frames->sleepTimeRemain)
		frames->sleepTimeRemain--;
	else
	{
		frames->sleepTimeRemain = frames->sleepTimeTotal;
		frames = frames->next;
		ASSERT(frames);
	}
}


//----------------------------------------------------------------------------
void IPixmap :: DrawTransClip(IDisplay& display, _LONG X, _LONG Y)
{
	ASSERT(frames);

	IRGBColor oldtranscolor = display.TransColor();
	display.SetTransColor(frames->transcolor);
	display.DrawTransClip(frames->pic, X, Y, frames->width, frames->height);
	display.SetTransColor(oldtranscolor);
	if (frames->sleepTimeRemain) {
		frames->sleepTimeRemain--;
	}
	else {
		frames->sleepTimeRemain = frames->sleepTimeTotal;
		frames = frames->next;
		ASSERT(frames);
	}
}


//----------------------------------------------------------------------------
void IPixmap :: Draw(IDisplay& display, _LONG X, _LONG Y, _BOOL Trans, _BOOL Clip)
{
	ASSERT(frames);
	//ASSERT(remaped);

	IRGBColor oldtranscolor = display.TransColor();
	display.SetTransColor(frames->transcolor);
	display.Draw(frames->pic, X, Y, frames->width, frames->height, Trans, Clip);
	display.SetTransColor(oldtranscolor);
	if (frames->sleepTimeRemain) {
		frames->sleepTimeRemain--;
	}
	else {
		frames->sleepTimeRemain = frames->sleepTimeTotal;
		frames = frames->next;
		ASSERT(frames);
	}
}


//----------------------------------------------------------------------------
_DWORD IPixmap :: Height() const
{
	ASSERT(frames);
	return frames->height;
}


//----------------------------------------------------------------------------
_DWORD IPixmap :: Width() const
{
	ASSERT(frames);
	return frames->width;
}


//----------------------------------------------------------------------------
void IPixmap :: Draw(IPixmap*B, _DWORD X, _DWORD Y)
{
	VALIDATETHIS;
	ASSERT(B);
	ASSERT(B->frames);
	ASSERT(frames);
	if ((frames->bpp != B->frames->bpp) ||
		(X>frames->width) || (Y>frames->height)) return;
	_BYTE *dest = frames->pic+X*frames->bpp + Y*frames->width*frames->bpp;
	_DWORD xmax = MIN(B->frames->width, frames->width-X);
	_DWORD ymax = MIN(B->frames->height, frames->height-Y);
	for (_DWORD y=0; y<ymax; y++) {
		Memory::Copy(dest+y*frames->width*frames->bpp,
				B->frames->pic+y*B->frames->width*frames->bpp,
				xmax*frames->bpp);
	}
}



//----------------------------------------------------------------------------
IPixmap* IPixmap :: GetRect(_DWORD X, _DWORD Y, _DWORD W, _DWORD H)
{
	VALIDATETHIS;
	ASSERT(frames);
	if ((Y>frames->height) || (X>frames->width)) return NULL;
	IPixmap *res = new IPixmap;
	IFrame *frm = res->AddFrame(frames->sleepTimeTotal);
	frm->height = MIN(H, frames->height-Y);
	frm->width = MIN(W, frames->width-X);
	frm->bpp = frames->bpp;
	if (frames->pal) 
		frm->pal = frames->pal->Copy();
	frm->pic = new _BYTE[frm->width*frm->height*frm->bpp];
	_BYTE *src = frames->pic+X*frames->bpp+Y*frames->width*frames->bpp;
	for (_DWORD y=0; y<frm->height; y++) {
		Memory::Copy(frm->pic+y*frm->width*frm->bpp, 
					src+y*frames->width*frames->bpp,
					frm->width*frames->bpp);
	}
	return res;
}


//----------------------------------------------------------------------------
void IPixmap :: Grab(IDisplay&D, _LONG &X, _LONG &Y, _DWORD &W, _DWORD &H)
{
	IFrame *frm = AddFrame(0);
	frm->pic = (_BYTE*) D.GetRect(X, 
								  Y, 
								  W, 
								  H);
	frm->width = W;
	frm->height = H;
	frm->bpp = (_BYTE)IColorSpace::colorSpace.BytesPerPixel();
}


//----------------------------------------------------------------------------
void IPixmap :: PutPixel(_DWORD X, _DWORD Y, const IRGBColor& COL)
{
	_DWORD c = IColorSpace::colorSpace.Color(COL);
	Memory::Copy(&frames->pic[X*frames->bpp+Y*frames->width*frames->bpp], 
			&c, frames->bpp);
}
