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
            
$Id: IDisplayCGFX.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <graphics/gfxbase.h>
#include <intuition/intuitionbase.h>
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>
#include <clib/exec_protos.h>
#include <cybergraphx/cybergraphics.h>
#include <clib/cybergraphics_protos.h>
#ifndef WARPUP
    #include <inline/cybergraphics.h>
#endif
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Interface.hpp>
#include "IDisplayAmiga.hpp"
#include "IDisplayCGFX.hpp"

#define CYBERGFXVER	41L
struct Library    	*CyberGfxBase = NULL;

_DWORD IDisplayCGFX::sInst = 0;

// -------------------------------
IDisplayCGFX* IDisplayCGFX::Create()
{
    if (!sInst++) {
        if ((CyberGfxBase = OpenLibrary(CYBERGFXNAME,CYBERGFXVER))==NULL) {
            CloseLibrary (&GfxBase->LibNode);
            Debug(dprintf("Can't open \"%s\" version %ld or newer.",
						  CYBERGFXNAME,CYBERGFXVER);)
            return NULL;
        }
    }
    return new IDisplayCGFX(DISPLAY_DOUBLE);
}


// -------------------------------
IDisplayCGFX::IDisplayCGFX(DisplayType T): IDisplayAmiga(T) {}


// -------------------------------
IDisplayCGFX :: ~IDisplayCGFX()
{
    if (!--sInst) {
		if (CyberGfxBase) {
			CloseLibrary(CyberGfxBase);
		}
	}
}


// -------------------------------
_BOOL IDisplayCGFX::CreateBuffers()
{
	if ((mRastPort == NULL) || (mWindow == NULL)) {
		return TRUE;
	}
    if (!GetCyberMapAttr(mRastPort->BitMap, CYBRMATTR_ISCYBERGFX)) {
		throw NMOException("Window ins't Cybergraphics compatible");
	}
	DispWidth = mWindow->Width - mWindow->BorderLeft - mWindow->BorderRight;
	DispHeight = mWindow->Height - mWindow->BorderTop - mWindow->BorderBottom;
    
	if (!(mDrawBitmap = AllocBitMap(DispWidth, DispHeight, 1, 
								   BMF_MINPLANES | BMF_DISPLAYABLE | BMF_CLEAR, 
								   mRastPort->BitMap)))    {
		dprintf("IDisplayCGFX: can't alloc double buffer bitmap\n");
		return FALSE;
	}
    
	TagItem tagLock[] = {LBMI_BYTESPERROW, (ULONG) &DispPitch, 
						LBMI_BASEADDRESS, (ULONG) &DrawSurface,
						TAG_DONE};
	APTR h = LockBitMapTagList(mDrawBitmap, tagLock);
	UnLockBitMap(h);    
    
	// manually clear because of buggy BMF_CLEAR
	Memory::Fill(DrawSurface, 0xff, DispPitch*DispHeight);
    
	SetClip(0, 0,DispWidth-1, DispHeight-1);
	mBorderTop = mWindow->BorderTop;
	mBorderLeft = mWindow->BorderLeft;
	return IDisplayAmiga::CreateBuffers();
}


//-------------------------------------------------
_BOOL IDisplayCGFX::OpenWBScreen()
{
    if (sScreen) return TRUE;
    if (!IDisplayAmiga::OpenWBScreen()) return FALSE;

    IColorSpace::colorSpace.SetBitsPerPixel(GetCyberMapAttr(
                                            sScreen->RastPort.BitMap, 
                                            CYBRMATTR_DEPTH));    
    _DWORD BitsMaskRed = 0;
    _DWORD BitsMaskGreen = 0;
    _DWORD BitsMaskBlue = 0;

    switch (GetCyberMapAttr(sScreen->RastPort.BitMap, CYBRMATTR_PIXFMT))
    {
    case PIXFMT_ARGB32:
            BitsMaskRed = 0xFF0000;
            BitsMaskGreen = 0xFF00;
            BitsMaskBlue = 0xFF;
            //order = RGB;
            dprintf("ARGB32\n");
            break;
    case PIXFMT_RGBA32:
            BitsMaskRed = 0xFF000000;
            BitsMaskGreen = 0xFF0000;
            BitsMaskBlue = 0xFF00;
            //order = RGB;
            dprintf("RGBA32\n");
            break;
    case PIXFMT_BGRA32:
            BitsMaskRed = 0xFF00;
            BitsMaskGreen = 0xFF0000;
            BitsMaskBlue = 0xFF000000;
            //order = BGR;
            dprintf("BGRA32\n");
            break;
    case PIXFMT_RGB24:
            BitsMaskRed = 0xFF0000;
            BitsMaskGreen = 0xFF00;
            BitsMaskBlue = 0xFF;
            //order = RGB;
            dprintf("RGB24\n");
            break;
    case PIXFMT_BGR24:
            BitsMaskRed = 0xFF;
            BitsMaskGreen = 0xFF00;
            BitsMaskBlue = 0xFF0000;
            //order = BGR;
            dprintf("BGR24\n");
            break;
    case PIXFMT_RGB16:
            BitsMaskRed = 0xF800;
            BitsMaskGreen = 0x7E0;
            BitsMaskBlue = 0x1F;
            //order = RGB;
            dprintf("RGB16\n");
            break;
    case PIXFMT_RGB15:
            BitsMaskRed = 0x7C00;
            BitsMaskGreen = 0x3E0;
            BitsMaskBlue = 0x1F;
            //order = RGB;
            dprintf("RGB15\n");
            break;
    case PIXFMT_BGR16:
            BitsMaskRed = 0x1F;
            BitsMaskGreen = 0x7E0;
            BitsMaskBlue = 0xF800;
            //order = RGB;
            dprintf("BGR16\n");
            break;
    case PIXFMT_BGR15:
            BitsMaskRed = 0x1F;
            BitsMaskGreen = 0x3E0;
            BitsMaskBlue = 0x7C00;
            //order = BGR;
            dprintf("BGR15\n");
            break;
    case PIXFMT_RGB15PC:
            BitsMaskRed = 0x1F00;
            BitsMaskGreen = 0xE003;
            BitsMaskBlue = 0x7C;
            //order = GRB;
            dprintf("RGB15PC\n");
            break;
    case PIXFMT_BGR15PC:
            BitsMaskRed = 0x7C;
            BitsMaskGreen = 0xE003;
            BitsMaskBlue = 0x1F00;
            //order = GBR;
            dprintf("BGR15PC\n");
            break;
    case PIXFMT_RGB16PC:
            BitsMaskRed = 0x1F00;
            BitsMaskGreen = 0xE007;
            BitsMaskBlue = 0xF8;
            //order = GRB;
            dprintf("RGB16PC\n");
            break;
    case PIXFMT_BGR16PC:
            BitsMaskRed = 0xF8;
            BitsMaskGreen = 0xE007;
            BitsMaskBlue = 0x1F00;
            //order = GBR;
            dprintf("BGR16PC\n");
            break;
    case PIXFMT_LUT8:
            // Load default pal here
            break;
    default:
            dprintf("IDisplayCGFX: Pixel format not supported!\n");
            return FALSE;
    }

    if ((BitsMaskRed) && (BitsMaskGreen) && (BitsMaskBlue))
        IColorSpace::colorSpace.SetColorMasks(BitsMaskRed, 
                                                          BitsMaskGreen,
                                                          BitsMaskBlue);
    return TRUE;
}

#endif
