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

$Id: IDisplayMacOS.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include "IDisplayMacOS.hpp"
using namespace NMO;

_BOOL IDisplayMacOS::locked = FALSE;

// -------------------------------
IDisplayMacOS::IDisplayMacOS(DisplayType T, WindowRef W): 
	IDisplayChunky(T), windowport(NULL), drawport(NULL), gworld(NULL)
{
	window = W;
}


// -------------------------------
IDisplayMacOS :: ~IDisplayMacOS()
{
	DestroyBuffers();
}

//--------------------------------------------------------------
_BOOL IDisplayMacOS :: NativeDoubleBuffer() const
{
	return QDIsPortBuffered(windowport);
}


// -------------------------------
_BOOL IDisplayMacOS::CreateBuffers()
{
	VALIDATETHIS;
	Rect rect;

	windowport = GetWindowPort(window);
#if TARGET_API_MAC_CARBON == 1
	GetPortBounds(windowport, &rect);
#else
	rect.left = windowport.portRect.left;
	rect.right = windowport.portRect.right;
	rect.top = windowport.portRect.top;
	rect.bottom = windowport.portRect.bottom;
#endif

	//if (Type() == DISPLAY_DOUBLE) {	
		if (NewGWorld(&gworld,
						0,
						&rect,
						NULL,
						NULL,
						noNewDevice) != noErr) {
			dprintf("IDisplayMacOS: error creating GWorld\n");
			return FALSE;
		}	
		/*RgnHandle visRgn = NewRgn();
		RectRgn(visRgn, &rect);
		SetPortVisibleRegion(gworld, visRgn);
		DisposeRgn(visRgn);*/
		drawport = gworld;
	/*}
	else {
		drawport = windowport;
	}*/

	/*IWindow::Link::handles.Reset();
	while (!IWindow::Link::handles.End()) {
		IDisplayMacOS &disp = (IDisplayMacOS &)IWindow::Link::handles.Next().nmo->Display();
		disp.QueryPixmap();	
	}*/
	
	DispWidth = rect.right;
	DispHeight = rect.bottom;
#if TARGET_API_MAC_CARBON == 1
	PixMapHandle pixmap = GetPortPixMap(drawport);
#else
	PixMapHandle pixmap = drawport->portPixMap;
#endif
	DispPitch = GetPixRowBytes(pixmap) & 0x7FFF;
	DrawSurface = (_BYTE*)GetPixBaseAddr(pixmap);
	IColorSpace::colorSpace.SetBitsPerPixel(GetPixDepth(pixmap));		
	Memory::Fill(DrawSurface, 0xFF, DispPitch * DispHeight);

	/*Debug(dprintf("IDisplayMacOS: %x: %d,%d\n",
			GetPortBitMapForCopyBits(drawport),
			DispWidth,
			DispHeight);)*/

	switch (IColorSpace::colorSpace.BitsPerPixel()) {
	case 8:
		{
			IPalette &pal = IColorSpace::colorSpace.Palette();
			CTabPtr colorpal = *((*pixmap)->pmTable);
			_DWORD ncolors = colorpal->ctSize + 1;
			pal.Reset(ncolors);
			Debug(dprintf("IDisplayMacOS: %d colors in pixmaps\n", ncolors);)
			for (_DWORD idx = 0; idx < ncolors; idx++) {
				ColorSpec color = colorpal->ctTable[idx];
				pal.Set(idx, color.rgb.red, color.rgb.green, color.rgb.blue);
			}
		}
		break;
	case 16:
		IColorSpace::colorSpace.SetColorMasks(0x7C00, 0x03E0, 0x001F);
		break;
	case 32:
		IColorSpace::colorSpace.SetColorMasks(0xFF0000, 0x00FF00, 0x0000FF);
		break;
	default:
		Debug(dprintf("IDisplayMacOS: unsupported BPP mode\n");)
		return FALSE;
	}
	
	SetClip(0, 0, DispWidth-1, DispHeight-1);
	return TRUE;
}


// -------------------------------
void IDisplayMacOS :: Refresh()
{
	if (!QDDone(windowport)) { return; }
	if (locked) { return; }
	locked = TRUE;
	VALIDATETHIS;
//	if (Type() == DISPLAY_DOUBLE) {
		CGrafPtr oldport;
		CGrafPtr oldgworld;
		GDHandle oldgdevice;
		Rect srect;
#if TARGET_API_MAC_CARBON == 1
		GetPortBounds(windowport, &srect);
#else
		srect.left = windowport.portRect.left;
		srect.right = windowport.portRect.right;
		srect.top = windowport.portRect.top;
		srect.bottom = windowport.portRect.bottom;
#endif
		CalcVis(window);
		RgnHandle visRgn = NewRgn();
		GetPortVisibleRegion(windowport, visRgn);
		GetPort(&oldport);
		GetGWorld(&oldgworld, &oldgdevice);
		SetGWorld(gworld, 0);
		SetPort(windowport);
		CopyBits(GetPortBitMapForCopyBits(drawport), 
				GetPortBitMapForCopyBits(windowport),
				&srect, &srect, srcCopy, visRgn);
		DisposeRgn(visRgn);
	    if (QDIsPortBuffered(windowport)) {
	        QDFlushPortBuffer(windowport, NULL);
	    }
//	}
	locked = FALSE;
}


// -------------------------------
void IDisplayMacOS :: Refresh(const IRect&RECT)
{
	if (!QDDone(windowport)) { return; }
	if (locked) { return; }
	locked = TRUE;
//	Refresh();
	VALIDATETHIS;
//	if (Type() == DISPLAY_DOUBLE) { 
		_LONG left = MAX(RECT.left, 0);
		_LONG top = MAX(RECT.top, 0);
		_LONG right = MAX(0l, MIN(_LONG(RECT.left + _LONG(RECT.width)), _LONG(DispWidth)));
		_LONG bottom = MAX(0l, MIN(_LONG(RECT.top + _LONG(RECT.height)), _LONG(DispHeight)));
		if ((right > left) && (bottom > top)) {
			CGrafPtr oldport;
			CGrafPtr oldgworld;
			GDHandle oldgdevice;
			Rect srect;
			SetRect(&srect, left, top, right, bottom);
			CalcVis(window);
			RgnHandle visRgn = NewRgn();
			GetPortVisibleRegion(windowport, visRgn);
			GetPort(&oldport);
			GetGWorld(&oldgworld, &oldgdevice);
			SetGWorld(gworld, 0);
			SetPort(windowport);
			CopyBits(GetPortBitMapForCopyBits(drawport), 
					GetPortBitMapForCopyBits(windowport),
					&srect, &srect, srcCopy, visRgn);
			SetGWorld(oldgworld, oldgdevice);
			SetPort(oldport);
	    	if (QDIsPortBuffered(windowport)) {
		        QDFlushPortBuffer(windowport, visRgn);
		    }
			DisposeRgn(visRgn);
		}
//	}
	locked = FALSE;
}



// -------------------------------
void IDisplayMacOS::DestroyBuffers()
{
	VALIDATETHIS;
	if (gworld != NULL) { 
		DisposeGWorld(gworld); 
		gworld = NULL;
	}
}


//--------------------------------------------------------------
/*_DWORD IDisplayMacOS :: Height() const 
{ 
	Rect rect;
	GetPortBounds(drawport, &rect);
	return rect.bottom;
}*/


//--------------------------------------------------------------
/*_DWORD IDisplayMacOS :: Width() const 
{ 
	Rect rect;
	GetPortBounds(drawport, &rect);
	return rect.right;
}*/


//--------------------------------------------------------------
/*void IDisplayMacOS :: SetPenColor(const IRGBColor& C) 
{
	forecolor.red = C.red << 8;
	forecolor.green = C.green << 8;
	forecolor.blue = C.blue << 8;
	SetGWorld(drawport, device);
	RGBForeColor(&forecolor);
}*/


//--------------------------------------------------------------
/*void IDisplayMacOS :: SetTransColor(const IRGBColor& C) 
{ 
}*/


//--------------------------------------------------------------
/*void IDisplayMacOS::GetClip(_LONG&X, _LONG&Y, _DWORD&W, _DWORD&H)
{
	X = cliprect.left;
	Y = cliprect.top;
	W = cliprect.right - cliprect.left;
	H = cliprect.bottom - cliprect.top;
}*/


//--------------------------------------------------------------
/*void IDisplayMacOS :: SetClip(_LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	SetGWorld(drawport, device);
	SetRect(&cliprect, X, Y, X + W, Y + H);
	ClipRect(&cliprect);
}*/


// -------------------------------
/*void IDisplayMacOS :: PenColor(IRGBColor&COLOR) 
{ 
	COLOR.red = forecolor.red >> 8;
	COLOR.green = forecolor.green >> 8;
	COLOR.blue = forecolor.blue >> 8;
}*/


// -------------------------------
/*void IDisplayMacOS :: TransColor(IRGBColor&COLOR) 
{ 
}*/


// -------------------------------
_BYTE *IDisplayMacOS :: GetRect(_LONG& X, _LONG& Y, _DWORD& W, _DWORD& H)
{
	QueryPixmap();
	_BYTE *res = IDisplayChunky::GetRect(X, Y, W, H);
	UnlockPixels(GetPortPixMap(drawport));
	return res;      
}


// -------------------------------
void IDisplayMacOS :: DrawNoTransNoClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	QueryPixmap();
	IDisplayChunky::DrawNoTransNoClip(Src, X, Y, W, H);
	UnlockPixels(GetPortPixMap(drawport));
}


// -------------------------------
void IDisplayMacOS :: DrawTransNoClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	QueryPixmap();
	IDisplayChunky::DrawTransNoClip(Src, X, Y, W, H);
	UnlockPixels(GetPortPixMap(drawport));
}


// -------------------------------
void IDisplayMacOS :: DrawNoTransClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	QueryPixmap();
	IDisplayChunky::DrawNoTransClip(Src, X, Y, W, H);
	UnlockPixels(GetPortPixMap(drawport));
}


// -------------------------------
void IDisplayMacOS :: DrawTransClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	QueryPixmap();
	IDisplayChunky::DrawTransClip(Src, X, Y, W, H);
	UnlockPixels(GetPortPixMap(drawport));
}


// -------------------------------
void IDisplayMacOS :: GetPixel (_LONG X, _LONG Y, IRGBColor&COL)
{
	QueryPixmap();
	IDisplayChunky::GetPixel(X, Y, COL);
	UnlockPixels(GetPortPixMap(drawport));
}


// -------------------------------
void IDisplayMacOS :: DrawPixel (_LONG X, _LONG Y)
{
	QueryPixmap();
	IDisplayChunky::DrawPixel(X, Y);
	UnlockPixels(GetPortPixMap(drawport));
}



// -------------------------------
void IDisplayMacOS :: DrawLine (_LONG x1, _LONG y1, _LONG x2, _LONG y2)
{
	QueryPixmap();
	IDisplayChunky::DrawLine(x1, y1, x2, y2);
	UnlockPixels(GetPortPixMap(drawport));
}


// -------------------------------
void IDisplayMacOS :: Clear()
{
	QueryPixmap();
	IDisplayChunky::Clear();
	UnlockPixels(GetPortPixMap(drawport));
}


// -------------------------------
void IDisplayMacOS :: DrawRect(_LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	QueryPixmap();
	IDisplayChunky::DrawRect(X, Y, W, H);
	UnlockPixels(GetPortPixMap(drawport));
}


// -------------------------------
void IDisplayMacOS :: DrawHLine (_LONG X, _LONG Y, _DWORD W)
{
	QueryPixmap();
	IDisplayChunky::DrawHLine(X, Y, W);
	UnlockPixels(GetPortPixMap(drawport));
}


#endif
