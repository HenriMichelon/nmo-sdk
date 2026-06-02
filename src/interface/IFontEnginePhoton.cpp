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

$Id: IFontEnginePhoton.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifdef __QNX__
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;
#include <Ap.h>
#include <Ph.h>
#include <Pt.h>

typedef struct 
{
	FontID	*id;
	uchar_t	*name;
	_DWORD	ascent;
	_DWORD	descent;
} FontEnginePhotonAbstract;

#define _m(x) ((FontEnginePhotonAbstract*)(this->mIFontEngineAbstract))->x


//---------------------------------------
IFontEngine::IFontEngine()
{
	palette = NULL;
	mIFontEngineAbstract = new FontEnginePhotonAbstract;
}


//---------------------------------------
IFontEngine::~IFontEngine()
{
	delete (FontEnginePhotonAbstract*)mIFontEngineAbstract;
}


//---------------------------------------
_BOOL IFontEngine::OpenFont(const Stringz &NAME, _DWORD SIZE, 
							_BOOL B, _BOOL I, _BOOL U, _BOOL A)
{
    Stringz name(NAME);
    if (!NAME.Len()) { name = "Helvetica"; }

    uint32_t size = SIZE;
    if (size == 0) { size = 12; }

    uint32_t weight = 0;
    if (B) { weight |= PF_STYLE_BOLD; }
    if (I) { weight |= PF_STYLE_ITALIC; }

	_m(id) = PfFindFont((uchar_t *)(_CHAR*)name,
						weight,
						size);

	if (_m(id) == NULL) {
		Debug(dprintf("IFontEngine: can't load font %s\n", (_CHAR*)name);)
		return FALSE;
	}
	_m(name) = PfConvertFontID(_m(id));

	FontQueryInfo fqinfo;
	if (PfQueryFontInfo((const char*)_m(name), &fqinfo) != 0) {
		Debug(dprintf("IFontEngine: can't query font info for %s\n", _m(name));)
		return FALSE;
	}
	Debug(dprintf("IFontEngine: font %s loaded\n", _m(name));)
	height = size;
	ymin = 0; //fqinfo.descender;
	_m(ascent) = fqinfo.ascender;
	_m(descent) = fqinfo.descender;
	antialiasing = FALSE;
	return TRUE;
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
	if (palette != NULL) { delete palette; }
	palette = new IPalette(2);
	palette->Set(0, bgColor);
	palette->Set(1, fgColor);
	return TRUE;
}


//---------------------------------------
void IFontEngine::FillCache(ICachedChar &car, const UChar &CHAR)
{
	PHFONT_METRICS	metrics;
	PhPoint_t 		*points;
	int				*ppiloops;

	if (PfGetOutline(_m(name),
					 (_WORD)CHAR,
					 &metrics,
					 &points,
					 &ppiloops) == -1) {
		dprintf("IFontEngine: error getting outline\n");
		return;
	}
	free(points);
	free(ppiloops);

	FontRender frender;
	if (PfGlyph((char*)_m(name),
				(_WORD)CHAR,
				&frender,
				NULL,
				0,
				(FontName)_m(name)) == -1) {
		dprintf("IFontEngine: error getting glyph \n");
		return;
	}
	_DWORD w = frender.width;
	_DWORD h = frender.size.y;
	//dprintf("w, h: %dx%d, %d, %d, %x\n", w, h, frender.bpl, frender.bpp, frender.bmptr);
	_BYTE *pic = new _BYTE[h * frender.bpl];
	if (PfGlyph((char*)_m(name),
				(_WORD)CHAR,
				&frender,
				pic,
				h * frender.bpl,
				(FontName)_m(name)) == -1) {
		dprintf("IFontEngine: error getting glyph \n");
		return;
	}
	car.bmp = new IPixmap;
	IPixmap::IFrame *frm= car.bmp->AddFrame();
	frm->height = h;
	frm->width = w;
	frm->pic = new _BYTE[frm->width * frm->height];
	Memory::Fill(frm->pic, 1, frm->width*frm->height);

	_DWORD idxoffset = 8 - (w % 8);
	_DWORD idx = 0;
	for (_DWORD y = 0; y < h; y ++) {
		_DWORD idxpic = 0;
		for (_DWORD x = 0; x < frender.bpl; x ++) {
			for (_BYTE i = 0; i < (8 - idxoffset) ; i++) {
				frm->pic[y * w + (idxpic++)] = ((pic[idx] >> (8 - i - 1)) & 1);
			}
			idx++;
		}
	}
	delete []pic;
    frm->sleepTimeTotal = 0;
    frm->sleepTimeRemain = 0;	
	frm->next = frm;
	frm->bpp = 1;
	frm->bpp = IColorSpace::colorSpace.Remap((_PTR&)frm->pic, 
											 palette, 
											 frm->width*frm->height, 
											 frm->bpp);
	car.xAdvance = w; // (metrics.Advance + 0xFFFFL) >> 16;
	car.height = h;
	car.ascent = 0; //(-metrics.BearingY + 0xFFFFL) >> 16 + h;
	car.descent = car.ascent;
	car.leftbearing =  0; //(metrics.BearingX + 0xFFFFL) >> 16;
	//dprintf("a, d, l: %d, %d,%d,%d\n", _m(ascent), car.xAdvance, car.leftbearing, _m(descent));
}

#endif
