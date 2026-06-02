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
                        
$Id: IDisplayAmiga.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>
#include <clib/exec_protos.h>
#include <graphics/gfxbase.h>
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Interface.hpp>
#include "IDisplayAmiga.hpp"
#include "IDisplayCGFX.hpp"

struct GfxBase * NMO::GfxBase = NULL;

_DWORD IDisplayAmiga::sInst = 0;
Screen *IDisplayAmiga::sScreen = NULL;

// -------------------------------
IDisplayAmiga* IDisplayAmiga::Create()
{
	if (!sInst++) {
		GfxBase = (struct GfxBase *) OpenLibrary(GRAPHXNAME, GRAPHXVERSION );
		if (!GfxBase) {
			Debug(dprintf("Can't open \"%s\" version %ld or newer.", 
						  GRAPHXNAME, GRAPHXVERSION);)
			return NULL;
		}
#if ! defined(AUTO_LIB)	  
		if (!IntuitionBase) {
			IntuitionBase = (struct IntuitionBase*) OpenLibrary(INTUITIONNAME, 
																INTUITIONVER);
		}
		if (!IntuitionBase) {
			Debug(dprintf("Can't open \"%s\" version %ld or newer.", 
						  INTUITIONNAME, INTUITIONVER);)
			return NULL;
		}            
#endif
	}
	IDisplayCGFX *cgfx = IDisplayCGFX::Create();
	return (IDisplayAmiga*)cgfx;
}


// -------------------------------
IDisplayAmiga::IDisplayAmiga(DisplayType T): IDisplayChunky(T) 
{
	mDrawBitmap = NULL;
	mWindow = NULL;
	mRastPort = NULL;
	mBorderLeft = 0;
	mBorderTop = 0;
}


// -------------------------------
IDisplayAmiga :: ~IDisplayAmiga()
{
    if (!--sInst) {
        if (sScreen) CloseScreen(sScreen);
        if (GfxBase) CloseLibrary (&GfxBase->LibNode);
    }
}


// -------------------------------
void IDisplayAmiga :: Refresh()
{
	if (mRastPort && mDrawBitmap) {
		BltBitMapRastPort(mDrawBitmap, 
    	                  0, 0, 
        	              mRastPort, 
            	          mBorderLeft, mBorderTop, 
                	      DispWidth, DispHeight, 
                    	  0x0C0); //0x9AF);
	}
}


// -------------------------------
void IDisplayAmiga :: Refresh(const IRect&RECT)
{
	if (mRastPort && mDrawBitmap) {
    	_LONG left = MAX(RECT.left, 0);
 	   _LONG top = MAX(RECT.top, 0);
    	_LONG right = MAX(0, MIN(_LONG(RECT.left + _LONG(RECT.width)), 
								 _LONG(DispWidth)));
	    _LONG bottom = MAX(0, MIN(_LONG(RECT.top + _LONG(RECT.height)), 
								  _LONG(DispHeight)));
    	if ((right > left) && (bottom > top)) {
    		BltBitMapRastPort(mDrawBitmap, 
     	                     left, top, 
        	                  mRastPort, 
            	              mBorderLeft + left, 
                	          mBorderTop + top, 
                    	      right - left, 
                        	  bottom - top, 
                         	 0x0C0); //0x9AF);
		}
    }
}



//-------------------------------------------------
_BOOL IDisplayAmiga::OpenWBScreen()
{
	if (sScreen == NULL) {
		char pubscreen[MAXPUBSCREENNAME];
		GetDefaultPubScreen(pubscreen);
		sScreen = LockPubScreen(pubscreen);
		UnlockPubScreen(pubscreen, sScreen);
	    IScreen::SetSize(sScreen->Width, 
						 sScreen->Height);
	}
	return TRUE;
}


// -------------------------------
void IDisplayAmiga::DestroyBuffers()
{
	if (mDrawBitmap != NULL) {
		FreeBitMap(mDrawBitmap);
		mDrawBitmap = NULL;
	}
}


//-------------------------------------------------
_BOOL IDisplayAmiga::CreateBuffers()
{
    /*::InitRastPort(&drawRastPort);
    drawRastPort.BitMap = DrawBitmap;
    LONG pen = ObtainBestPen(NULL, 200, 200, 200, NULL);
    
    ::RectFill(&drawRastPort, 0, 0, DispWidth, DispHeight);
    ReleasePen(NULL, pen);*/
    return TRUE;
}


//-------------------------------------------------
void IDisplayAmiga :: SetWindow (Window *WIN)
{
	ASSERT(WIN);
	mWindow = WIN;
	mRastPort = mWindow->RPort;
}

#endif
