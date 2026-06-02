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

	(c) 1999-2001 Henri Michelon 

$Id: FileGif.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/interface/IRect.hpp>
#include <nmo/interface/IRGBColor.hpp>
#include <nmo/interface/IPalette.hpp>
#include <nmo/interface/IDisplay.hpp>
#include <nmo/interface/IPixmap.hpp>
#include <nmo/datatypes/FilePixmap.hpp>
#include <nmo/datatypes/FileGif.hpp>
#include "modules/Modungif.hpp"
using namespace NMO;


//------------------------------------------------
IPixmap *FileGif :: Load(Stream &STREAM, DynModule *MODULE)
{
	return NULL;
/*	ASSERT(MODULE);
	Modungif *ungif = (Modungif*)MODULE;
	GifFileType	*gifFile = (*ungif->DGifOpenFileName)((char*)N);
	if (gifFile == NULL) {
		//Debug(dprintf("FileGif: Cannot load %s\n", (char*)N);)
		return NULL;
	}
	if ((*ungif->DGifSlurp)(gifFile) == GIF_ERROR) { 
		//Debug(dprintf("FileGif: %s: %d\n", (char*)N, ungif->GifLastError());)
		return NULL;
	}
	IPixmap *T = new IPixmap;
	for (_LONG index=0; index<gifFile->ImageCount; index++)
	{
		ColorMapObject *colorMap;
		IPixmap::IFrame *bf = T->AddFrame(0);
	    SavedImage *sp = &gifFile->SavedImages[index];		
		bf->width = LE2LONG(sp->ImageDesc.Width);
		bf->height = LE2LONG(sp->ImageDesc.Height);
		if (sp->ImageDesc.ColorMap)
			colorMap = sp->ImageDesc.ColorMap;
		else
			colorMap = gifFile->SColorMap;
		//dprintf("gif bpp: %d\n", colorMap->BitsPerPixel);
		bf->bpp = LE2LONG(colorMap->BitsPerPixel) / 8;
		if (!bf->bpp) bf->bpp = 1;
		_DWORD size = bf->width * bf->height * bf->bpp;
		bf->pic = new _BYTE[size];
		Memory::Copy(bf->pic, sp->RasterBits, size);
		bf->pal = new IPalette(colorMap->ColorCount);
		bf->pal->MapRGB((_BYTE*)colorMap->Colors, colorMap->ColorCount);
		//dprintf("%dx%dx%d (%d)\n", bf->width, bf->height, bf->bpp, bf->pal->NbColors());
	}
	//(*ungif->DGifCloseFile(gifFile);
	return T;*/
}

