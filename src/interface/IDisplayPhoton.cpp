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

$Id: IDisplayPhoton.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifdef __QNX__
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include <Pt.h>
#include <photon/Pg.h>
#include "IDisplayPhoton.hpp"
using namespace NMO;

/*extern "C" {
int PgGetVideoMode( PgDisplaySettings_t *settings );
int PgGetVideoModeInfo( unsigned short mode_number,
                        PgVideoModeInfo_t *mode_info );
}*/

_DWORD IDisplayPhoton::nInst = 0;


// -------------------------------
IDisplayPhoton::IDisplayPhoton(DisplayType T, PtWidget_t *W): 
IDisplayChunky(T) , window(W)
{
}



// -------------------------------
IDisplayPhoton :: ~IDisplayPhoton()
{
	DestroyBuffers();
}


// -------------------------------
void IDisplayPhoton :: RealRefresh(const IRect&RECT)
{
	PhPoint_t p = { 0, 0 };
	PhRect_t rect;
	rect.ul.x = RECT.left;
	rect.ul.y = RECT.top;
	rect.lr.x = RECT.left + RECT.width;
	rect.lr.y = RECT.top + RECT.height;
	PgSetClipping(1, &rect);
	PgDrawImage(phimage.image,
				phimage.type,
				&p,
				&phimage.size,
				phimage.bpl,
				0);
}


// -------------------------------
void IDisplayPhoton :: Refresh()
{
	PtDamageWidget(window);
}


// -------------------------------
void IDisplayPhoton :: Refresh(const IRect&RECT)
{
	Refresh();
}



// -------------------------------
void IDisplayPhoton::DestroyBuffers()
{
	PhReleaseImage(&phimage);
}



// -------------------------------
_BOOL IDisplayPhoton::CreateBuffers()
{
	VALIDATETHIS;
	if (! nInst++) {
		PgVideoModeInfo_t	mode_info;
		PgDisplaySettings_t	display_settings;
		PgGetVideoMode(&display_settings);
		PgGetVideoModeInfo(display_settings.mode ,&mode_info);
		IScreen::SetSize(mode_info.width, mode_info.height);

		Debug(dprintf("Photon screen: %dx%dx%d (%d)\n", 
				  mode_info.width,
				  mode_info.height,
				  mode_info.bits_per_pixel,
				  mode_info.type);)
		IColorSpace::colorSpace.SetBitsPerPixel(mode_info.bits_per_pixel);
	}

	int imagemode;
    switch (IColorSpace::colorSpace.BitsPerPixel()) {
    case 8:
        {
			imagemode = Pg_IMAGE_DIRECT_444;
            IPalette &pal = IColorSpace::colorSpace.Palette();
            _DWORD ncolors = 0;
            pal.Reset(ncolors);
            Debug(dprintf("IDisplayPhoton: %d colors in pixmaps\n", ncolors);)
            for (_DWORD idx = 0; idx < ncolors; idx++) {
                //pal.Set(idx, .red, .green, .blue);
            }
        }
        break;
    case 15:
		dprintf("Photon image mode is DIRECT_555\n");
		imagemode = Pg_IMAGE_DIRECT_555;
        IColorSpace::colorSpace.SetColorMasks(0x7C00, 0x03E0, 0x001F);
		break;
    case 16:
		dprintf("Photon image mode is DIRECT_565\n");
		imagemode = Pg_IMAGE_DIRECT_565;
        IColorSpace::colorSpace.SetColorMasks(0x7C00, 0x07E0, 0x001F);
        break;
    case 24:
    case 32:
		imagemode = Pg_IMAGE_DIRECT_888;
        IColorSpace::colorSpace.SetColorMasks(0xFF0000, 0x00FF00, 0x0000FF);
        break;
    default:	
		throw new NMOException("Unsupported BPP");
		break;
	}

	PhArea_t *area;
    PtGetResource(window, Pt_ARG_AREA, &area, 0);
	Debug(dprintf("PhWidget area: %dx%d,%dx%d\n",
				area->pos.x, area->pos.y,
				area->size.w, area->size.h));
	DispWidth = area->size.w;
	DispHeight = area->size.h;
	phimage.image = NULL;
	DispPitch = area->size.w;
	if (DispPitch % 8) DispPitch += 8 - (DispPitch % 8);
	PhCreateImage(&phimage,
                  DispPitch,
                  DispHeight,
                  imagemode,
                  NULL,
                  0,
                  1);
	DispPitch = DispPitch * IColorSpace::colorSpace.BytesPerPixel();
	DrawSurface = (_BYTE*)phimage.image;
	SetClip(0, 0, DispWidth, DispHeight);
	Debug(dprintf("PhImage data located at 0x%x\n", DrawSurface);)
	Memory::Fill(DrawSurface, 0xFF, DispPitch * DispHeight);
	return TRUE;
}
#endif
