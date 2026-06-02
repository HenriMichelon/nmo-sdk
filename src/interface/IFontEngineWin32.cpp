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
		
$Id: IFontEngineWin32.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <nmo/Interface.hpp>
# include <windows.h>
#endif

FIXED FontEngineWin32FixedFromDouble(double);

class AbstractFontEngineWin32
{
private:
	MAT2	mat2;
	HFONT	font;
	FIXED FixedFromDouble(double d);
	friend class IFontEngine;
};

#define __m(x) ((AbstractFontEngineWin32*)(this->mIFontEngineAbstract))->x


//---------------------------------------
FIXED AbstractFontEngineWin32::FixedFromDouble(double d)
{
    long l = (long) (d * 65536L);
    return *(FIXED *)&l;
}


//---------------------------------------
IFontEngine::IFontEngine(): palette(NULL)
{
	mIFontEngineAbstract = new AbstractFontEngineWin32;
	__m(mat2).eM11 = __m(FixedFromDouble)(1.0);
	__m(mat2).eM12 = __m(FixedFromDouble)(0.0);
	__m(mat2).eM21 = __m(FixedFromDouble)(0.0);
	__m(mat2).eM22 = __m(FixedFromDouble)(1.0);
}


//---------------------------------------
IFontEngine::~IFontEngine()
{
	if (palette) delete palette;
	DeleteObject(__m(font));
	delete (AbstractFontEngineWin32*)mIFontEngineAbstract;
}


//---------------------------------------
_BOOL IFontEngine::OpenFont(const Stringz &NAME, const _DWORD SIZE, const _BOOL B, 
							const _BOOL I, const _BOOL U, const _BOOL A)
{
	Stringz name;
	if (!NAME.Len()) {
		//name = "MS Sans Serif";
		name = "Verdana";
		//name = "Courier New";
	}
	else {
		name = NAME;
	}

	_LONG size = SIZE;
	if (!size)
		size = 11;

	int weight = FW_NORMAL;
	if (B)
		weight = FW_BOLD;

	HDC dc = GetDC(NULL);
	if (!dc) return NULL;
	__m(font) = CreateFont(-size, 0, 0, 0, weight, I, U, FALSE, DEFAULT_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_DONTCARE, name);
	if (SelectObject(dc, __m(font)) == NULL)
	{
		dprintf("IFontEngine::OpenFont(): SelectObject()\n");
		ReleaseDC(NULL, dc);
		return NULL;
	}
	TEXTMETRIC textmetric;
	GetTextMetrics(dc, &textmetric);
	height = textmetric.tmAscent + textmetric.tmDescent;
	ymin = textmetric.tmDescent;
	antialiasing = A;		
	ReleaseDC(NULL, dc);
	return __m(font) != NULL;
}


//---------------------------------------
_BOOL IFontEngine::SetSize(_DWORD)
{
	return FALSE;
}


//---------------------------------------
_BOOL IFontEngine::Bold(const _BOOL B)
{
	return FALSE;
}


//---------------------------------------
_BOOL IFontEngine::Italic(const _BOOL I)
{
	return FALSE;
}


//---------------------------------------
_BOOL IFontEngine::Underlined(const _BOOL U)
{
	return FALSE;
}


//---------------------------------------
_BOOL IFontEngine::SetColors(const IRGBColor &fgColor, const IRGBColor &bgColor)
{
	if (palette) delete palette;
	/*palette = new Palette(5);
	palette->Set(0, bgColor);
	palette->Set(4, fgColor);
	if (antialiasing)
	{
		RGBColor diff(Abs(bgColor.red - fgColor.red),
					Abs(bgColor.green - fgColor.green),
					Abs(bgColor.blue - fgColor.blue));
		palette->Set(1, RGBColor((fgColor.red + diff.red/2+diff.red/4)%256,
								(fgColor.green + diff.green/2+diff.green/4)%256,
								(fgColor.blue + diff.blue/2+diff.blue/4)%256));
		palette->Set(2, RGBColor((fgColor.red + diff.red/2)%256,
								(fgColor.green + diff.green/2)%256,
								(fgColor.blue + diff.blue/2)%256));
		palette->Set(3, RGBColor((fgColor.red + diff.red/3)%256,
								(fgColor.green + diff.green/3)%256,
								(fgColor.blue + diff.blue/3)%256));
	}
	else
	{
		palette->Set(1, bgColor);
		palette->Set(2, fgColor);
		palette->Set(3, fgColor);
	}*/
	palette = new IPalette(2);
	palette->Set(0, bgColor);
	palette->Set(1, fgColor);
	return TRUE;
}

extern "C" {
USHORT GetTTUnicodeGlyphIndex(HDC hdc, USHORT ch);
USHORT GetTTUnicodeCharCount(HDC hdc);
}

//---------------------------------------
void IFontEngine::FillCache(ICachedChar &car, const UChar& CHAR)
{
	ASSERT(palette);
	GLYPHMETRICS gm;

	HDC dc = GetDC(NULL);
	if (!dc) return;
	if (SelectObject(dc, __m(font)) == NULL)	{
		dprintf("IFontEngine::FillCache(): SelectObject()\n");
		ReleaseDC(NULL, dc);
		return;
	}
	

	//UINT dchar = GetTTUnicodeGlyphIndex(dc, _WORD(CHAR));
	UINT dchar = _WORD(CHAR);
	_DWORD size = GetGlyphOutlineA(dc, dchar, 
									GGO_BITMAP, // + GGO_GLYPH_INDEX, 
									&gm, 0, NULL, &__m(mat2));

	if (size == GDI_ERROR)	{
		size = GetGlyphOutline(dc, 20, GGO_BITMAP, &gm, 0, NULL, &__m(mat2));
		if (size == GDI_ERROR)	{
			ReleaseDC(NULL, dc);
			return;
		}
	}
	car.bmp = new IPixmap;
	IPixmap::IFrame *frm= car.bmp->AddFrame(0);

	frm->height = gm.gmBlackBoxY;
	if (size)	{
		if ((size*4) > (gm.gmBlackBoxX*gm.gmBlackBoxY))
			frm->width = gm.gmBlackBoxX+(size*4 - gm.gmBlackBoxX*gm.gmBlackBoxY)/gm.gmBlackBoxY;
		else
			frm->width = gm.gmBlackBoxX-(gm.gmBlackBoxX*gm.gmBlackBoxY - size*4)/gm.gmBlackBoxY;
		//dprintf("%d: %d %d / %d %d\n", size, frm->width, frm->height, gm.gmBlackBoxX, gm.gmBlackBoxY);
		frm->pic = new _BYTE[frm->width*frm->height];
		BYTE *pic = new _BYTE[size];
		GetGlyphOutline(dc, _WORD(dchar), GGO_BITMAP, &gm, size, pic, &__m(mat2));
		Memory::Fill(frm->pic, 0, frm->width*frm->height);


		_DWORD idx = 0;
		for (_DWORD y=0; y<frm->height; y++) {
			for (_DWORD x=0; x<(frm->width/8); x++) {
				for (_BYTE i=0; i<8; i++) {
					frm->pic[y*frm->width + x*8 + (7-i)] = (pic[idx] >> i) & 1;
					//dprintf("%d ", frm->pic[y*frm->width + x*8 + (7-i)]);
				}
				idx++;
			}
			idx += (frm->width/4) - (frm->width/8);
			//dprintf("\n");
		}
		delete []pic;
	}
	else {
		frm->width = gm.gmBlackBoxX;
		frm->pic = new _BYTE[frm->width*frm->height];
		Memory::Fill(frm->pic, 0, frm->width*frm->height);
	}
	frm->sleepTimeTotal = 0;
	frm->sleepTimeRemain = 0;
	frm->next = frm;
	frm->bpp = 1;
	frm->bpp = (_BYTE)IColorSpace::colorSpace.Remap((_PTR&)frm->pic, palette, frm->width*frm->height, frm->bpp);
	car.leftbearing =  gm.gmptGlyphOrigin.x;
	car.xAdvance = gm.gmCellIncX; // + car.leftbearing;
	car.height = gm.gmBlackBoxY;
	car.ascent = gm.gmptGlyphOrigin.y;
	car.descent = gm.gmBlackBoxY-gm.gmptGlyphOrigin.y;
	ReleaseDC(NULL, dc);
}

#endif
