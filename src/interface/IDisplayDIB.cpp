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

$Id: IDisplayDIB.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
-------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <nmo/Thread.hpp>
# include <nmo/Interface.hpp>
# include <windows.h>
#endif
#include "SimpleDIB.hpp"
#include "IDisplayDIB.hpp"
#include "nmo/Interface/DefaultPal.h"

#define DIBERROR 0xffffffff

// -------------------------------
IDisplayDIB :: IDisplayDIB(DisplayType T, HWND hWndApp, HINSTANCE hInst):
	IDisplayChunky(T), hwnd(hWndApp), hInstance(hInst) 
{
}



// -------------------------------
IDisplayDIB::~IDisplayDIB()
{
	DeleteObject(hrgn);
	DeleteObject(hpen);
	DeleteObject(hbrush);
	DestroyBuffers();
}


// -------------------------------
void IDisplayDIB :: DestroyBuffers()
{
	dblBuffer.Destroy();
}


// -------------------------------
BOOL IDisplayDIB :: MakePalette(IPalette&PAL)
{
	if(m_hPalette != NULL) ::DeleteObject(m_hPalette);
	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2*sizeof(WORD)+
													256 * sizeof(PALETTEENTRY)];
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = 256;
	for(int i = 0; i < 256; i++) {
		pLogPal->palPalEntry[i].peRed = PAL[i].red;
		pLogPal->palPalEntry[i].peGreen = PAL[i].green;
		pLogPal->palPalEntry[i].peBlue = PAL[i].blue;
		pLogPal->palPalEntry[i].peFlags = 0;
	}
	m_hPalette = ::CreatePalette(pLogPal);
	delete pLogPal;
	return TRUE;
}



// -------------------------------
_BOOL IDisplayDIB :: CreateBuffers()
{
	ASSERT(this);

	RECT wrect;
	HDC	wdc;
	if (!::GetClientRect(hwnd, &wrect))
	{
		dprintf("DIB Initialize: cannot get main window client RECT\n");
		return FALSE;
	}

	wdc = ::GetDC(hwnd);
	if (!wdc) {
		dprintf("DIB Initialize: cannot get main window DC\n");
		return FALSE;
	}

	IColorSpace::colorSpace.SetBitsPerPixel(GetDeviceCaps(wdc, BITSPIXEL));
	if (!wrect.right) wrect.right = 1;
	if (!wrect.bottom) wrect.bottom = 1;
	if (!dblBuffer.CreateDIB(wdc, wrect.right, wrect.bottom, 
		(_BYTE)IColorSpace::colorSpace.BitsPerPixel())) {
		//csDisplay->Unlock();
		return FALSE;
	}
	DispWidth = dblBuffer.sizeDIB.cx;
	DispHeight = dblBuffer.sizeDIB.cy;
	DrawSurface = dblBuffer.GetBits();
	DispPitch = DispWidth *  IColorSpace::colorSpace.BytesPerPixel();
	if (DispPitch % 4) DispPitch += 4-(DispPitch % 4);
	Memory::Fill(DrawSurface, 0xFF, DispPitch * DispHeight);

	switch (IColorSpace::colorSpace.BitsPerPixel()) {
	case 8:
		{
			IPalette &pal = IColorSpace::colorSpace.Palette();
			if (pal.Count() > 0) {
				if (!MakePalette(pal)) {
					Debug(dprintf("DIB Initialize: MakePalette failed %d\n", GetLastError());)
					return FALSE;
				}
				if (!dblBuffer.SetPalette(m_hPalette)) {
					Debug(dprintf("DIB Initialize: SetPalette failed %d\n", GetLastError());)
					return FALSE;
				}
				if (!::SelectPalette(wdc, m_hPalette, FALSE)) {
					Debug(dprintf("DIB Initialize: SelectPalette failed %d\n", GetLastError());)
					return FALSE;
				}
				if (!::RealizePalette(wdc))	{
					Debug(dprintf("DIB Initialize: RealizePalette failed %d\n", GetLastError());)
					return FALSE;
				}
			}
			else {
				PALETTEENTRY entries[256];
				_DWORD ncolors = GetSystemPaletteEntries(wdc, 0, 256, entries);
				pal.Reset(ncolors);
				for (_DWORD i=0; i<ncolors; i++) {
					pal.Set(i, entries[i].peRed,
							entries[i].peGreen,
							entries[i].peBlue);
				}
			}
		}
		break;
	case 15:
	case 16:
		IColorSpace::colorSpace.SetColorMasks(0x7C00, 0x03E0, 0x001F);
		break;
	case 24:
	case 32:
		IColorSpace::colorSpace.SetColorMasks(0xFF0000, 0x00FF00, 0x0000FF);
		break;
	default:
		dprintf("DIB Initialize: unsupported BPP mode\n");
		ASSERT(FALSE);
	}

	::ReleaseDC(hwnd, wdc);
	SetClip(0, 0, DispWidth, DispHeight);
	//Debug(dprintf("DIB mode selected: %dx%dx%d - %d\n", DispWidth, DispHeight, 
	//		IColorSpace::colorSpace.BitsPerPixel(), DispPitch);)
	return TRUE;
}


// -------------------------------
void IDisplayDIB :: Refresh()
{
	HDC wdc = ::GetDC(hwnd);
	dblBuffer.BitsToDevice(wdc, 0, 0);
	::ReleaseDC(hwnd, wdc);
}


// -------------------------------
void IDisplayDIB :: Refresh(const IRect&RECT)
{
	_LONG left = MAX(RECT.left, 0);
	_LONG top = MAX(RECT.top, 0);
	_LONG right = MAX(0l, MIN(_LONG(RECT.left + _LONG(RECT.width)), _LONG(DispWidth)));
	_LONG bottom = MAX(0l, MIN(_LONG(RECT.top + _LONG(RECT.height)), _LONG(DispHeight)));
	/*dprintf("refresh: %d,%d,%d,%d -> %d,%d,%d,%d\n",
				RECT.left, RECT.top, RECT.width, RECT.height,
				left, top, right - left, bottom - top);*/
	if ((right > left) && (bottom > top)) {
		HDC wdc = ::GetDC(hwnd);
		dblBuffer.BitsToDevice(wdc, left, top, 
									left, top, 
									right - left, 
									bottom - top);
		::ReleaseDC(hwnd, wdc);
	}
}


// -------------------------------
void IDisplayDIB :: SetPenColor(const IRGBColor&COL)
{
	COLORREF newcolor = RGB(COL.red, COL.green, COL.blue);
	if (newcolor != pencolor) {
		pencolor = newcolor;
		DeleteObject(hbrush);
		hbrush = CreateSolidBrush(pencolor);
		HPEN newpen = CreatePen(PS_SOLID, 1, pencolor);
		SelectObject(dblBuffer.GetDC(), newpen);
		DeleteObject(hpen);
		hpen = newpen;
		IDisplayChunky::SetPenColor(COL);
	}
}


// -------------------------------
void IDisplayDIB :: SetClip(_LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	DeleteObject(hrgn);
	hrgn = CreateRectRgn(X, DispHeight - Y, X+W, DispHeight - Y - H);
	SelectClipRgn(dblBuffer.GetDC(), hrgn);
	IDisplayChunky::SetClip(X, Y, W, H);
}


// -------------------------------
void IDisplayDIB :: Clear()
{
	DrawRect(0, 0, DispWidth, DispHeight);
}


// -------------------------------
void IDisplayDIB :: DrawPixel (_LONG X, _LONG Y)
{
	DrawLine(X, Y, X+1, Y+1);
}


// -------------------------------
void IDisplayDIB :: DrawLine (_LONG X1, _LONG Y1, _LONG X2, _LONG Y2)
{
	HDC hdc = dblBuffer.GetDC();
	MoveToEx(hdc, X1, DispHeight - Y1-1, NULL);
	LineTo(hdc, X2, DispHeight - Y2-1);
}


// -------------------------------
void IDisplayDIB :: DrawHLine (_LONG X, _LONG Y, _DWORD W)
{
	DrawLine(X, Y, X + W, Y);
}


// -------------------------------
void IDisplayDIB :: DrawRect(_LONG L, _LONG T, _DWORD W, _DWORD H)
{
	RECT rect;
	SetRect(&rect, L, DispHeight - T, 
			L+W, DispHeight - T - H);
	FillRect(dblBuffer.GetDC(), &rect, hbrush);
}

#endif
