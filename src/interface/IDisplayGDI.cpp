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

	2D GDI based display device

	(c) 1998-2002 Henri Michelon

$Id: IDisplayGDI.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/config.h>
#if defined(WIN32) || defined(_WIN32)
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <nmo/Interface/DefaultPal.h>
#include "IDisplayGDI.hpp"
using namespace NMO;

// -------------------------------
IDisplayGDI :: IDisplayGDI(HWND hWndApp, HINSTANCE hInstanceApp):
	brush(NULL), pen(NULL), hrgn(NULL), IDisplayChunky()
{
	hwnd = hWndApp;
	hInstance = hInstanceApp;
	SetPenColor(0);
}



// -------------------------------
IDisplayGDI::~IDisplayGDI()
{
	DestroyBuffer();
}


// -------------------------------
void IDisplayGDI :: DestroyBuffer(void)
{
	ASSERT(this);
	if(m_hPalette) 
	{
		::DeleteObject(m_hPalette);
		m_hPalette = NULL;
	}
}



// -------------------------------
BOOL IDisplayGDI :: MakePalette()
{
	ASSERT(this);
	if(m_hPalette != NULL) ::DeleteObject(m_hPalette);
	LPLOGPALETTE pLogPal = (LPLOGPALETTE) new char[2*sizeof(WORD)+
									256 * sizeof(PALETTEENTRY)];
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = 256;
	for(int i = 0; i < 256; i++) {
		pLogPal->palPalEntry[i].peRed = DefaultPal[i*3];
		pLogPal->palPalEntry[i].peGreen = DefaultPal[i*3+1];
		pLogPal->palPalEntry[i].peBlue = DefaultPal[i*3+2];
		pLogPal->palPalEntry[i].peFlags = 0;
	}
	m_hPalette = ::CreatePalette(pLogPal);
	ASSERT(m_hPalette);
	delete pLogPal;
	return TRUE;
}	



// -------------------------------
_BOOL IDisplayGDI :: Initialize()
{
	ASSERT(this);

	RECT wrect;
	if (!::GetClientRect(hwnd, &wrect))
	{
		dprintf("DIB Initialize: cannot get main window client RECT\n");
		return FALSE;
	}

	HDC	wdc = ::GetDC(hwnd);
	if (!wdc)
	{
		dprintf("DIB Initialize: cannot get main window DC\n");
		return FALSE;
	}

	IColorSpace::colorSpace.SetBitsPerPixel(GetDeviceCaps(wdc, BITSPIXEL));
	DispWidth = wrect.right;
	DispHeight = wrect.bottom;
	DispPitch = DispWidth *  IColorSpace::colorSpace.BytesPerPixel();
	if (DispPitch % 4) DispPitch += 4-(DispPitch % 4);
	SetClip(0, 0, DispWidth-1, DispHeight-1);

	switch (IColorSpace::colorSpace.BitsPerPixel())
	{
	case 8:
		if (!MakePalette()) 
		{
			dprintf("DIB Initialize: MakePalette failed %d\n", GetLastError());
			return FALSE;
		}
		if (!::SelectPalette(wdc, m_hPalette, FALSE))
		{
			dprintf("DIB Initialize: SelectPalette failed %d\n", GetLastError());
			return FALSE;
		}
		if (!::RealizePalette(wdc))
		{
			dprintf("DIB Initialize: RealizePalette failed %d\n", GetLastError());
			return FALSE;
		}
		break;
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
   int iSize = sizeof(BITMAPINFOHEADER) ;
   memset(&BIH, 0, iSize);
   BIH.biSize = iSize;
   BIH.biPlanes = 1;
   BIH.biBitCount = IColorSpace::colorSpace.BitsPerPixel();
   BIH.biCompression = BI_RGB;

	::ReleaseDC(hwnd, wdc);
	//dp("DIB mode selected: %dx%dx%d - %d\n", DispWidth, DispHeight, BitsPerPixel, DispPitch); 
	return TRUE;
}



// -------------------------------
void IDisplayGDI :: DrawNoTransNoClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	ASSERT(this);
	BIH.biWidth = W;
	BIH.biHeight = -(_LONG)H;
	HDC	wdc = ::GetDC(hwnd);
	SelectClipRgn(wdc, hrgn);
	::SetDIBitsToDevice(wdc, X, Y, W, H, 0, 0, 0, H, Src, (LPBITMAPINFO)&BIH, DIB_RGB_COLORS);
	::ReleaseDC(hwnd, wdc);
}



// -------------------------------
void IDisplayGDI :: DrawTransNoClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	ASSERT(this);
	BIH.biWidth = W;
	BIH.biHeight = -(_LONG)H;
	HDC	wdc = ::GetDC(hwnd);
	SelectClipRgn(wdc, hrgn);
	::SetDIBitsToDevice(wdc, X, Y, W, H, 0, 0, 0, H, Src, (LPBITMAPINFO)&BIH, DIB_RGB_COLORS);
	::ReleaseDC(hwnd, wdc);
}



// -------------------------------
void IDisplayGDI :: DrawNoTransClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	ASSERT(this);
	BIH.biWidth = W;
	BIH.biHeight = -(_LONG)H;
	HDC	wdc = ::GetDC(hwnd);
	SelectClipRgn(wdc, hrgn);
	::SetDIBitsToDevice(wdc, X, Y, W, H, 0, 0, 0, H, Src, (LPBITMAPINFO)&BIH, DIB_RGB_COLORS);
	::ReleaseDC(hwnd, wdc);
}


// -------------------------------
void IDisplayGDI :: DrawTransClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	ASSERT(this);
	BIH.biWidth = W;
	BIH.biHeight = -(_LONG)H;
	HDC	wdc = ::GetDC(hwnd);
	SelectClipRgn(wdc, hrgn);
	::SetDIBitsToDevice(wdc, X, Y, W, H, 0, 0, 0, H, Src, (LPBITMAPINFO)&BIH, DIB_RGB_COLORS);
	::ReleaseDC(hwnd, wdc);
}



// -------------------------------
void IDisplayGDI :: ClearPage()
{
	ASSERT(this);
	HDC	wdc = GetDC(hwnd);
	SelectClipRgn(wdc, hrgn);
	SendMessage(hwnd, WM_ERASEBKGND, (WPARAM) wdc, 0);
	ReleaseDC(hwnd, wdc);
}



// -------------------------------
void IDisplayGDI :: DrawLine (_LONG X1, _LONG Y1, _LONG X2, _LONG Y2)
{
	HDC	wdc = GetDC(hwnd);
	//SelectClipRgn(wdc, hrgn);
	HPEN open = SelectObject(wdc, pen);
	MoveToEx(wdc, X1, Y1, NULL);
	LineTo(wdc, X2, Y2);
	SelectObject(wdc, open);
	ReleaseDC(hwnd, wdc);
}


// -------------------------------
void IDisplayGDI :: SetPenColor(_DWORD C)
{
	_BYTE r, g, b;
	IColorSpace::colorSpace.Color(C, r, g, b);
	IDisplay::SetPenColor(C);

	HDC	wdc = GetDC(hwnd);
	if (pen) DeleteObject(pen);
	if (brush) DeleteObject(brush);
	pen = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	brush = CreateSolidBrush(RGB(r, g, b));
	ReleaseDC(hwnd, wdc);
}



// -------------------------------
void IDisplayGDI :: DrawHLine (_LONG X, _LONG Y, _DWORD W)
{
	DrawLine(X, Y, X+W, Y);
}


// -------------------------------
void IDisplayGDI :: DrawRect(_LONG X1, _LONG Y1, _LONG X2, _LONG Y2)
{
	HDC	wdc = GetDC(hwnd);
	HPEN pen = CreatePen(PS_NULL, 1, 0);
	HPEN open = SelectObject(wdc, pen);
	HBRUSH obrush = SelectObject(wdc, brush);
	SelectClipRgn(wdc, hrgn);
	Rectangle(wdc, X1, Y1, X1 + X2+1, Y1 + Y2+1);
	SelectObject(wdc, open);
	SelectObject(wdc, obrush);
	ReleaseDC(hwnd, wdc);
}



// -------------------------------
void IDisplayGDI :: SetClip(_DWORD X1, _DWORD Y1, _DWORD W, _DWORD H)
{
	IDisplay::SetClip(X1, Y1, W, H);
	if (hrgn) DeleteObject(hrgn);
	hrgn = CreateRectRgn(XClipStart, YClipStart, XClipEnd, YClipEnd);
}
#endif
