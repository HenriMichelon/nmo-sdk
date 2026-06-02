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

	(c) 2000-2001 Henri Michelon

$Id: IFontEngineFreeType.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/config.h>
#ifdef HAVE_FREETYPE
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;
#include "freetype.h"

static _BYTE virtual_palette[5] = { 5, 4, 3, 2, 1 };

typedef struct {
	TT_Face				face;
	TT_Glyph			glyph;
	TT_Engine			engine;
	TT_CharMap			unicode_char_map;
	TT_Instance			instance;
	TT_Instance_Metrics	imetrics;
	TT_Face_Properties  properties;
} AsbtractFontFreeType;

#define __m(x) ((AsbtractFontFreeType*)(this->mIFontEngineAbstract))->x


//---------------------------------------
IFontEngine::IFontEngine(): palette(NULL)
{
	mIFontEngineAbstract = new AsbtractFontFreeType;
	if (TT_Init_FreeType(&__m(engine))) {
		throw NMOException("FreeType : TT_Init_FreeType");
	}
	TT_Set_Raster_Gray_Palette(__m(engine), virtual_palette);
	//dprintf("Using FreeType 1 font engine\n");
}


//---------------------------------------
IFontEngine::~IFontEngine()
{
	TT_Done_FreeType(__m(engine));
	delete (AsbtractFontFreeType*)mIFontEngineAbstract;
	if (palette) { delete palette; }
}


//---------------------------------------
_BOOL IFontEngine::OpenFont(const Stringz &NAME, _DWORD SIZE, _BOOL, 
						 _BOOL, _BOOL, _BOOL)
{
	Stringz name = NAME;
	if (!name.Len()) { name = "verdana.ttf"; }
	_DWORD size = SIZE;
	if (!size) { size = 8; }
	
    unsigned short  i, n;
    unsigned short  platform, encoding;

    _BOOL res = (TT_Open_Face(__m(engine), name, &__m(face)) !=
				TT_Err_Could_Not_Open_File );
	if (!res) {
		name = Dir::Current() + PATHSEP + name;
		_BOOL res = (TT_Open_Face(__m(engine), name, &__m(face)) !=
					TT_Err_Could_Not_Open_File );
		if (!res) {
			Debug(dprintf("IFontEngine: error loading %s\n", (_CHAR*)name);)
			return FALSE;
		}
	}
	res = res && ( TT_Get_Face_Properties(__m(face), &__m(properties)) == TT_Err_Ok );
    res = res && ( TT_New_Glyph(__m(face), &__m(glyph)) == TT_Err_Ok );
    res = res && ( TT_New_Instance(__m(face), &__m(instance)) == TT_Err_Ok );
    res = res && ( TT_Set_Instance_Resolutions(__m(instance), 96, 96) == TT_Err_Ok );
	if (!res) return FALSE;
	SetSize(12);

    /* look for a Unicode charmap */
    n = __m(properties).num_CharMaps;
    for ( i = 0; i < n; i++ )
    {
		TT_Get_CharMap_ID(__m(face), i, &platform, &encoding );
		if ( (platform == 3 && encoding == 1 )  ||
           (platform == 0 && encoding == 0 ) )
		{
			TT_Get_CharMap(__m(face), i, &__m(unicode_char_map) );
			i = n + 1;
		}
    }
    res = res && (i != n);
    if (!TT_Set_Instance_CharSize(__m(instance), size*64)) {
	    TT_Get_Instance_Metrics(__m(instance), &__m(imetrics));
	}
	height = (__m(properties).header->yMax + __m(properties).header->yMin) / 64 /2;
	ymin = (-__m(properties).header->yMin)/64/2;
    return res;
}


//---------------------------------------
_BOOL IFontEngine::SetSize(_DWORD)
{
	return FALSE;
}


//---------------------------------------
_BOOL IFontEngine::Bold(_BOOL)
{
	return FALSE;
}


//---------------------------------------
_BOOL IFontEngine::Italic(_BOOL)
{
	return FALSE;
}


//---------------------------------------
_BOOL IFontEngine::Underlined(_BOOL)
{
	return FALSE;
}


//---------------------------------------
_BOOL IFontEngine::SetColors(const IRGBColor &fgColor, const IRGBColor &bgColor)
{
	if (palette) delete palette;
	palette = new IPalette(2);
	palette->Set(0, bgColor);
	palette->Set(1, fgColor);
	return TRUE;
}


//---------------------------------------
void IFontEngine::FillCache(ICachedChar &car, const UChar &CHAR)
{
	ASSERTMSG(palette, "Select foreground & background colors before printing text");

    TT_Glyph_Metrics	metrics;
	TT_Raster_Map		Bit;
	short glyph_code = TT_Char_Index(__m(unicode_char_map), (short)CHAR);

    if (TT_Load_Glyph(__m(instance), __m(glyph), glyph_code, TTLOAD_DEFAULT))
		return;
    if (TT_Get_Glyph_Metrics(__m(glyph), &metrics))
		return;
    Bit.width  = (metrics.bbox.xMax - metrics.bbox.xMin) / 64;
    Bit.rows   = (metrics.bbox.yMax - metrics.bbox.yMin) / 64;
    Bit.flow   = TT_Flow_Up;
    Bit.cols   = (Bit.width+3) & -4;  /* must be 32-bits aligned */
    Bit.size   = (long)Bit.cols * Bit.rows;
    Bit.bitmap = new _BYTE[(int)Bit.size];
    Memory::Fill(Bit.bitmap, 0, Bit.size);
	if (TT_Get_Glyph_Pixmap(__m(glyph), &Bit, -metrics.bbox.xMin, -metrics.bbox.yMin))
		return;

	car.bmp = new IPixmap;
	IPixmap::IFrame *frm= car.bmp->AddFrame(0);
	frm->width = metrics.advance / 64;
	frm->height = MAX(Bit.rows, 1l);
	frm->next = frm;
	frm->bpp = 1;
	frm->pic = new _BYTE[frm->height*frm->width];
	frm->pal = palette->Clone();
	Memory::Fill(frm->pic, 0, frm->height*frm->width);

	_LONG xx, yy;
	_BYTE *src = (_BYTE*)Bit.bitmap;
	_LONG adv = metrics.bearingX / 64;
	for (yy=0; yy<Bit.rows; yy++) {
		for (xx=0; xx<Bit.cols; xx++) {
			if ((src[(Bit.rows-yy-1)*Bit.cols+xx] == 1) ||
				(src[(Bit.rows-yy-1)*Bit.cols+xx] == 2) ||
				(src[(Bit.rows-yy-1)*Bit.cols+xx] == 3))
				frm->pic[xx+(yy*frm->width)+adv] = 1;
		}
	}
	delete []Bit.bitmap;
	car.bmp->Remap();

	car.leftbearing = 0; //metrics.bearingY / 64;
	car.xAdvance = metrics.advance / 64;
	car.height = (metrics.bbox.yMax - metrics.bbox.yMin) / 64;
	car.ascent = metrics.bearingY / 64;
	car.descent = car.height -  car.ascent; //metrics.bbox.yMin / 64;
}
#endif
