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

$Id: IDisplayDirectDraw.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
-------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <nmo/Thread.hpp>
# include <nmo/Interface.hpp>
# include <windows.h>
# include <ddraw.h>
#endif
#include "IDisplayDirectDraw.hpp"
#include "nmo/Interface/DefaultPal.h"


// -------------------------------
IDisplayDirectDraw :: IDisplayDirectDraw(DisplayType T, 
										 HWND hWndApp, 
										 HINSTANCE hInstanceApp,
										 LPDIRECTDRAW lpDDApp):
	hwnd(hWndApp), 
	hInstance(hInstanceApp),
	lpDD(lpDDApp),
	lpDDSPrimary(NULL),
	lpDDSBack(NULL),
	IDisplayChunky(T) 
{
		//flipFlag = FALSE;
}


// -------------------------------
_BOOL IDisplayDirectDraw::CreateBuffers()
{
    //DDSCAPS		ddscaps;		// DirectDraw Surface Capabilities
	DDSURFACEDESC	ddsd;			// DirectDraw Surface Description

    // Create the primary surface with 1 back buffer
	Memory::Fill(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof( ddsd );
    /*ddsd.dwBackBufferCount = 1;
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |
									DDSCAPS_FLIP |
								    DDSCAPS_COMPLEX;
									//| DDSCAPS_VIDEOMEMORY;*/
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY;;
    HRESULT ddrval = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL );
    if( ddrval != DD_OK ) {
        ddsd.ddsCaps.dwCaps &= ~DDSCAPS_VIDEOMEMORY;
	    ddrval = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL );
		if( ddrval != DD_OK ) {
			Debug(dprintf ("DirectDraw: Unable to create primary surface #%d\n", ddrval);)
			return FALSE;
		}
	}

	/*Memory::Fill(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	lpDDSPrimary->Lock(NULL, &ddsd, 0, NULL);
	IColorSpace::colorSpace.SetBitsPerPixel(ddsd.ddpfPixelFormat.dwRGBBitCount);
	DispPitch = ddsd.lPitch;
	primarySurface = (BYTE*) ddsd.lpSurface;
	lpDDSPrimary->Unlock(ddsd.lpSurface);*/

	RECT rect;
	GetClientRect(hwnd, &rect);
	DispWidth = rect.right - rect.left;
	DispHeight = rect.bottom - rect.top;

	lpDD->CreateClipper(0, &lpddClipper, NULL);
	lpddClipper->SetHWnd(0, hwnd);
	lpDDSPrimary->SetClipper(lpddClipper);


	// Get a pointer to the back buffer
    /*ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack);*/
	Memory::Fill(&ddsd, 0, sizeof(ddsd));
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN |DDSCAPS_VIDEOMEMORY ;
	ddsd.dwWidth = DispWidth;
	ddsd.dwHeight = DispHeight;
    ddrval = lpDD->CreateSurface(&ddsd, &lpDDSBack, NULL );
    if( ddrval != DD_OK ) {
        ddsd.ddsCaps.dwCaps &= ~DDSCAPS_VIDEOMEMORY;
	    ddrval = lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL );
		if( ddrval != DD_OK ) {
			dprintf ("error DirectDraw: Unable to get Attached Surface\n");
			return FALSE;
		}
	}

	Memory::Fill(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	lpDDSBack->Lock(NULL, &ddsd, 0, NULL);
	DispPitch = ddsd.lPitch;
	DrawSurface = (BYTE*) ddsd.lpSurface;
	IColorSpace::colorSpace.SetBitsPerPixel(_BYTE(ddsd.ddpfPixelFormat.dwRGBBitCount));
	memset(DrawSurface, 0xff, ddsd.lPitch*ddsd.dwHeight);
	lpDDSBack->Unlock(ddsd.lpSurface);

	if (!(ddsd.ddpfPixelFormat.dwFlags & DDPF_RGB)) {
		dprintf("error DirectDraw: Only RGB modes allowed\n");
		return FALSE;
	}

	if (ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
		LPDIRECTDRAWPALETTE	lpDDPal;		// DirectDraw palette
		_DWORD nbcols = IColorSpace::colorSpace.Palette().Count();
		//dprintf("nbcols: %d\n", nbcols);
	    PALETTEENTRY* ape = new PALETTEENTRY[nbcols];
		for (_WORD i=0; i<nbcols; i++) {
			ape[i].peRed = IColorSpace::colorSpace.Palette()[i].red;
			ape[i].peGreen = IColorSpace::colorSpace.Palette()[i].green;
			ape[i].peBlue = IColorSpace::colorSpace.Palette()[i].blue;
			ape[i].peFlags = 0;
		}
		ddrval = lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE, ape, &lpDDPal, NULL);
		delete []ape;
		if (ddrval != DD_OK) {
			Debug(dprintf("error DirectDraw: cannot create palette %d\n", ddrval);)
			return FALSE;
		}
		ddrval = lpDDSPrimary->SetPalette(lpDDPal);
		if (ddrval != DD_OK) {
			Debug(dprintf("error DirectDraw: cannot set palette on primary surface%d\n", ddrval);)
			return FALSE;
		}
		ddrval = lpDDSBack->SetPalette(lpDDPal);
		if (ddrval != DD_OK) {
			Debug(dprintf("error DirectDraw: cannot set palette on back surface d\n", ddrval);)
			return FALSE;
		}
	}
	else {
		IColorSpace::colorSpace.SetColorMasks(ddsd.ddpfPixelFormat.dwRBitMask,
											ddsd.ddpfPixelFormat.dwGBitMask,
											ddsd.ddpfPixelFormat.dwBBitMask);
	}
	SetClip(0, 0, DispWidth-1, DispHeight-1);

    return TRUE;
}


// -------------------------------
void IDisplayDirectDraw :: DestroyBuffers()
{
	if( lpDDSBack != NULL) {
		lpDDSBack->Release();
        lpDDSBack = NULL;
    }
	if( lpDDSPrimary != NULL) {
		lpDDSPrimary->SetClipper(NULL);

		lpDDSPrimary->Release();
        lpDDSPrimary = NULL;
    }

}


// -------------------------------
IDisplayDirectDraw :: ~IDisplayDirectDraw()
{
	DestroyBuffers();
}



// -------------------------------
void IDisplayDirectDraw :: Refresh()
{
	/*HRESULT ddrval;
    while (1)
	{
		//lpDD->WaitForVerticalBlank(DDWAITVB_BLOCKEND, NULL);
        ddrval = lpDDSPrimary->Flip(NULL, DDFLIP_WAIT);
	    if( ddrval == DD_OK ) break;
        if( ddrval == DDERR_SURFACELOST )
	    {
			lpDDSPrimary->Restore();
			lpDDSBack->Restore();
			if( ddrval != DD_OK ) break;
        }
		if( ddrval != DDERR_WASSTILLDRAWING ) break;
	}
	if (flipFlag)
		DrawSurface = backSurface;
	else
		DrawSurface = primarySurface;
	flipFlag = !flipFlag;*/
    RECT rect;
    POINT p1;
    POINT p2;
    GetClientRect(hwnd, &rect);
    p1.x = rect.left;
    p1.y = rect.top;
    p2.x = rect.right;
    p2.y = rect.bottom;
    ClientToScreen(hwnd, &p1);
    ClientToScreen(hwnd, &p2);
    rect.left   = p1.x;
    rect.top    = p1.y;
    rect.right  = p2.x;
    rect.bottom = p2.y;
    lpDDSPrimary->Blt(&rect, lpDDSBack, NULL, DDBLT_ASYNC , NULL);
}


// -------------------------------
void IDisplayDirectDraw :: Refresh(const IRect&CRECT)
{
    RECT rect, srcrect;
    POINT p1;
    POINT p2;
    srcrect.left   = CRECT.left;
    srcrect.top    = CRECT.top;
    srcrect.right  = CRECT.width + CRECT.left;
    srcrect.bottom = CRECT.height + CRECT.top;
    p1.x = srcrect.left;
    p1.y = srcrect.top;
    p2.x = srcrect.right;
    p2.y = srcrect.bottom;
    ClientToScreen(hwnd, &p1);
    ClientToScreen(hwnd, &p2);
    rect.left   = p1.x;
    rect.top    = p1.y;
    rect.right  = p2.x;
    rect.bottom = p2.y;
    lpDDSPrimary->Blt(&rect, lpDDSBack, &srcrect, DDBLT_ASYNC , NULL);
}
#endif
