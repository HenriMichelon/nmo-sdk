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

$Id: FilePng.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/interface/IRect.hpp>
#include <nmo/interface/IRGBColor.hpp>
#include <nmo/interface/IPalette.hpp>
#include <nmo/interface/IDisplay.hpp>
#include <nmo/interface/IPixmap.hpp>
#include <nmo/datatypes/FilePixmap.hpp>
#include <nmo/datatypes/FilePng.hpp>
using namespace NMO;

#include <stdio.h>
#include "libpng/png.h"


void png_nmo_read_data(png_structp png_ptr, 
							  png_bytep data, 
							  png_size_t length) {
	Stream *stream = (Stream*)png_ptr->io_ptr;
	try {
		stream->Read(data, length);
	}
	catch (Stream::ExcStreamRead) {
		png_error(png_ptr, "Read Error");
	}
}


/*static void png_nmo_write_data(png_structp png_ptr, 
							  png_bytep data, 
							  png_size_t length) {
	File *file = png_ptr->io_ptr;
	if (file->Write(data, length) == -1) {
		png_error(png_ptr, "Write Error");
	}
}*/



//------------------------------------------------
IPixmap *FilePng :: Load(Stream&STREAM, DynModule*)
{
	png_byte header[8];
	STREAM.Read(&header, 8);
	if (png_sig_cmp(header, 0, 8)) {
		return NULL;
	}

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info = png_create_info_struct(png);

	png_set_read_fn(png, (png_voidp)&STREAM, png_nmo_read_data);
	png_set_sig_bytes(png, 8);
	png_read_info(png, info);
	_BYTE depth = png_get_bit_depth(png, info);
	_BYTE colortype = png_get_color_type(png, info);

	IPixmap *T = NULL;
	if (depth < 8) png_set_packing(png);
	if (depth == 16) png_set_strip_16(png);
	if (colortype & PNG_COLOR_MASK_ALPHA) png_set_strip_alpha(png);

	/* pixmap creation */
	T = new IPixmap;
	IPixmap::IFrame *bf = T->AddFrame();
	bf->width = png_get_image_width(png, info);
	bf->height = png_get_image_height(png, info);
	bf->bpp = (_BYTE)MAX(8, (_DWORD)png_get_bit_depth(png, info)) / 8;

	if (colortype == PNG_COLOR_TYPE_PALETTE) {
		png_color *palette;
		int ncolors;
		if (png_get_PLTE(png, info, &palette, &ncolors)) {
			Debug(dprintf("PNG: %dx%dx%d (%d), %d colors in palette, ", 
						  bf->width, bf->height, png_get_bit_depth(png, info),
						  bf->bpp, ncolors);)
			png_read_update_info(png, info);
			/* read indexed palette */
			bf->pal = new IPalette(ncolors);
			bf->pal->MapRGB((_BYTE*)palette, ncolors);
		}
	}
	else if ((colortype == PNG_COLOR_TYPE_RGB) ||
			 (colortype == PNG_COLOR_TYPE_RGB_ALPHA)) {
		bf->bpp *= 3;
		Debug(dprintf("PNG: %dx%dx%d (%d), RGB, ", 
					  bf->width, bf->height, 
					  png_get_bit_depth(png, info),
					  bf->bpp);)
	}

	/* Use transparency or background color */
	if (info->num_trans != 0) {
		bf->transcolor = IRGBColor(bf->pal->Get(info->num_trans-1));
		bf->trans = TRUE;
		Debug(dprintf("trans color (%d,%d,%d), ",
						bf->transcolor.red,
						bf->transcolor.green,
						bf->transcolor.blue);)
	}
	else {
		bf->transcolor = IRGBColor(_BYTE(info->background.red),
									_BYTE(info->background.green),
									_BYTE(info->background.blue));
		bf->trans = FALSE;
		Debug(dprintf("bg color (%d,%d,%d), ",
						bf->transcolor.red,
						bf->transcolor.green,
						bf->transcolor.blue);)
	}

	/* read PNG data */
	_DWORD idx;
	_DWORD rowbytes = png_get_rowbytes(png, info);
	Debug(dprintf("%d bytes in a row\n", rowbytes);)
	png_byte** rows = new png_byte*[bf->height];
	for (idx = 0; idx < bf->height; idx++) {
		rows[idx] = new _BYTE[rowbytes];
	}
	png_read_image(png, rows);
	_DWORD size = bf->width * bf->height * bf->bpp;
	bf->pic = new _BYTE[size];
	if ((colortype == PNG_COLOR_TYPE_RGB) ||
		(colortype == PNG_COLOR_TYPE_RGB_ALPHA)) {
		for (idx = 0; idx < bf->height; idx++) {
			Memory::Fill(bf->pic + idx * bf->width, 0, rowbytes);
			delete []rows[idx];
		}
	}
	else {
		for (idx = 0; idx < bf->height; idx++) {
			Memory::Copy(bf->pic + idx * bf->width,
						 rows[idx], rowbytes);
			delete []rows[idx];
		}
	}
	delete []rows;

	png_destroy_read_struct(&png, &info, NULL);
	return T;
}

