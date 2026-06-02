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

$Id: IDisplayBeOS.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifdef __BEOS__
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include "IDisplayBeOS.hpp"
using namespace NMO;

// -------------------------------
IDisplayBeOS::IDisplayBeOS(DisplayType T): IDisplayChunky(T) 
{
}



// -------------------------------
IDisplayBeOS :: ~IDisplayBeOS()
{
	DestroyBuffer();
}


// -------------------------------
void IDisplayBeOS :: Refresh()
{
}


// -------------------------------
void IDisplayBeOS :: Refresh(const IRect&RECT)
{
	Refresh();
}



// -------------------------------
void IDisplayBeOS::DestroyBuffer()
{
}



// -------------------------------
_BOOL IDisplayBeOS::CreateBuffer(IHandle window)
{
	VALIDATETHIS;

	BScreen screen;
	IScreen::SetSize(screen.Frame().right - screen.Frame().left, 
					screen.Frame().bottom - screen.Frame().top);	
	
    switch (screen.ColorSpace()) {
    case B_CMAP8:
        {
			IColorSpace::colorSpace.SetBitsPerPixel(8);
            IPalette &pal = IColorSpace::colorSpace.palette;
            _DWORD ncolors = 0;
            pal.Reset(ncolors);
            dprintf("IDisplayPhoton: %d colors in pixmaps\n", ncolors);
            for (_DWORD idx = 0; idx < ncolors; idx++) {
                //pal.Set(idx, .red, .green, .blue);
            }
        }
        break;
    case B_RGB15:
		IColorSpace::colorSpace.SetBitsPerPixel(15);
        IColorSpace::colorSpace.SetColorMasks(0x7C00, 0x03E0, 0x001F);
    case B_RGB16:
		IColorSpace::colorSpace.SetBitsPerPixel(15);
        IColorSpace::colorSpace.SetColorMasks(0x7C00, 0x07E0, 0x001F);
        break;
    case B_RGB32:
		IColorSpace::colorSpace.SetBitsPerPixel(32);
        IColorSpace::colorSpace.SetColorMasks(0xFF0000, 0x00FF00, 0x0000FF);
        break;
    default:	
		throw new NMOException("Unsupported BPP");
		break;
	}

	DispWidth = (_DWORD)ABS(window->Bounds().right - window->Bounds().left);
	DispHeight = (_DWORD)ABS(window->Bounds().bottom - window->Bounds().top);
	DispPitch = 0;
	SetClip(0, 0, DispWidth, DispHeight);
	DrawSurface = (_BYTE*)NULL;
	//Memory::Fill(DrawSurface, 0xFF, DispPitch * DispHeight);
	return TRUE;
}
#endif
