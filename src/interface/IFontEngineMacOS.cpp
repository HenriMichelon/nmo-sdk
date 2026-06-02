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

$Id: IFontEngineMacOS.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;

typedef struct {
	GWorldPtr	gworld;
	Rect		bbox;
	RGBColor	fgcolor;
	RGBColor	bgcolor;
	FontInfo	fontinfo;
	_DWORD		size;
	SInt16		fontID;
} FontEngineAbstract;

#define __m(x) ((FontEngineAbstract*)(this->mIFontEngineAbstract))->x

//---------------------------------------
IFontEngine::IFontEngine(): palette(NULL)
{
	mIFontEngineAbstract = new FontEngineAbstract;
	__m(gworld) = NULL;
	__m(bgcolor).red = 0xFF << 8;
	__m(bgcolor).green = 0xFF << 8;
	__m(bgcolor).blue = 0xFF << 8;
	__m(fgcolor).red = 0;
	__m(fgcolor).green = 0;
	__m(fgcolor).blue = 0;	
}


//---------------------------------------
IFontEngine::~IFontEngine()
{
	if (__m(gworld) != NULL) { DisposeGWorld(__m(gworld)); }
	delete (FontEngineAbstract*)mIFontEngineAbstract;
	if (palette) { delete palette; }
}


//---------------------------------------
_BOOL IFontEngine::OpenFont(const Stringz &NAME, _DWORD SIZE, _BOOL B, 
						 _BOOL  I, _BOOL U, _BOOL A)
{
	if (__m(gworld) != NULL) { 
		DisposeGWorld(__m(gworld)); 
		__m(gworld) = NULL;
	}

	FMFontFamily fam = 0;
	if (NAME.Len()) {
		Str255 pname;
		CopyCStringToPascal((_CHAR*)NAME, pname);
		fam = FMGetFontFamilyFromName(pname);
	}
	if (!fam) {
		fam = GetAppFont();
	}
	__m(fontID) = fam;

	__m(size) = SIZE;
	if (!__m(size)) { __m(size) = 10; }
	
	_DWORD style = 0;
	
	if (FetchFontInfo(__m(fontID), __m(size), style, &__m(fontinfo)) != noErr) {
		dprintf("IFontEngine: error geting font information\n");
		return FALSE;
	}
	SetRect(&__m(bbox), 0, 0, __m(fontinfo).widMax, 
			(__m(fontinfo).ascent + __m(fontinfo).descent ));

	if (NewGWorld(&__m(gworld),
					8,
					&__m(bbox),
					NULL,
					NULL,
					0) != noErr) {
		dprintf("IFontEngine: error creating GWorld\n");
		return FALSE;
	}
	height = __m(fontinfo).ascent + __m(fontinfo).descent;
	ymin = __m(fontinfo).descent;
	antialiasing = FALSE;
	
	GrafPtr oldport, oldgworld;
	GDHandle oldgdevice;
	GetPort(&oldport);
	GetGWorld(&oldgworld, &oldgdevice);
	SetGWorld(__m(gworld), 0);
	RGBForeColor(&__m(fgcolor));
	RGBBackColor(&__m(bgcolor));
	TextFont(__m(fontID));
	TextSize(__m(size));
	TextFace(style);
	SetGWorld(oldgworld, oldgdevice);
	SetPort(oldport);
	
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
	if (palette) { delete palette; }
	palette = new IPalette(2);
	palette->Set(0, bgColor);
	palette->Set(1, fgColor);
	return TRUE;
}


//---------------------------------------
void IFontEngine::FillCache(ICachedChar &car, const UChar &CHAR)
{
	UChar uchar = CHAR;
	CharParameter ch = uchar.To("APPLE_ROMAN");
	GrafPtr oldport, oldgworld;
	GDHandle oldgdevice;
	GetPort(&oldport);
	GetGWorld(&oldgworld, &oldgdevice);
	SetGWorld(__m(gworld), 0);
	EraseRect(&__m(bbox));
	MoveTo(0, __m(fontinfo).ascent);
	DrawChar(ch);
	SetGWorld(oldgworld, oldgdevice);
	SetPort(oldport);

	car.bmp = new IPixmap;
	if (!car.bmp) { return; }
	IPixmap::IFrame *frm= car.bmp->AddFrame();
	frm->bpp = 1;
	frm->height = __m(bbox).bottom;
	frm->width = CharWidth(ch);
	frm->pic = new _BYTE[frm->width*frm->height*frm->bpp];
	if (!frm->pic) {
		delete car.bmp;
		car.bmp = NULL;
		return; 
	}
	PixMapHandle pixmap = GetPortPixMap(__m(gworld));
	_DWORD pitch = GetPixRowBytes(pixmap) & 0x7FFF;
	_BYTE *surface = (_BYTE*)GetPixBaseAddr(pixmap);
	Memory::Fill(frm->pic, 0, frm->width*frm->height*frm->bpp);
	for (_DWORD y = 0; y < frm->height; y ++) {
		for (_DWORD x = 0; x < (frm->width - 1); x++) {
			if (surface[x + y * pitch] != 0) {
				frm->pic[x + y * frm->width] = 1;
			}
		}
	}
	//Memory::Fill(frm->pic, 1, frm->width*frm->height*frm->bpp);
	frm->bpp = IColorSpace::colorSpace.Remap((_PTR&)frm->pic, palette, frm->width * frm->height, frm->bpp);

	car.xAdvance = frm->width;
	car.height = frm->height;
	car.ascent = 0;
	car.descent = __m(fontinfo).descent;
	car.leftbearing = 0;
}

#endif
