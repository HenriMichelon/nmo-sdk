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

	(c) 1999-2002 Henri Michelon

$Id: IFontEngineX11.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/config.h>
#ifdef HAVE_X11
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/Interface.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "X11Win.hpp"
#include <stdio.h>

class IFontEngineX11Abstract
{
private:
	friend class IFontEngine;
	XFontStruct *font;
    XCharStruct defcs;
    GC          gc;

	IFontEngineX11Abstract();
	~IFontEngineX11Abstract();
};

#define _m(x) ((IFontEngineX11Abstract*)(this->mIFontEngineAbstract))->x



//---------------------------------------
IFontEngineX11Abstract::IFontEngineX11Abstract()
{
	font = NULL;
}

IFontEngineX11Abstract::~IFontEngineX11Abstract()
{
	if (font !=  NULL) {
#ifndef _LINUX
		XFreeFont(X11Win::display, font);
#endif
		XFreeGC(X11Win::display, gc);
		font = NULL;
	}
}


//---------------------------------------
IFontEngine::IFontEngine()
{
	mIFontEngineAbstract = NULL;
	palette = NULL;
}


//---------------------------------------
IFontEngine::~IFontEngine()
{
	if (palette != NULL) { 
		delete palette; 
		palette = NULL;
	}
	if (mIFontEngineAbstract != NULL) {
		delete (IFontEngineX11Abstract*)mIFontEngineAbstract;
		mIFontEngineAbstract = NULL;
	}
}


//---------------------------------------
_BOOL IFontEngine::OpenFont(const Stringz &NAME, _DWORD, _BOOL, 
						 _BOOL, _BOOL, _BOOL A)
{
	if (mIFontEngineAbstract == NULL) { mIFontEngineAbstract = new IFontEngineX11Abstract; }
	Stringz name;
	if (!NAME.Len()) {
		name = "8x13";
	}
	else {
		name = NAME;
	}

	if (!(_m(font) = XLoadQueryFont(X11Win::display, (_CHAR*)name))) {
		fprintf(stderr, "Cannot open font %s\n", (_CHAR*)name);
		Debug(dprintf("Cannot open font %s\n", (_CHAR*)name);)
		return FALSE;
	}
	XGCValues values;
	values.background = 0;
	values.foreground = 1;
	_m(gc) = XCreateGC(X11Win::display, 
					RootWindow(X11Win::display, X11Win::screen_num),
					GCForeground|GCBackground, &values);
	XSetFont(X11Win::display, _m(gc), _m(font)->fid);
	Debug(dprintf("IFontEngine: font %s loaded\n", (_CHAR*)name);)
	if (!_m(font)->per_char) {
		_m(defcs).lbearing = _m(font)->min_bounds.lbearing;
		_m(defcs).rbearing = _m(font)->max_bounds.rbearing;
		_m(defcs).width = _m(defcs).rbearing - _m(defcs).lbearing;
		_m(defcs).ascent = _m(font)->ascent;
		_m(defcs).descent = _m(font)->descent;
	}
	height = _m(font)->ascent + _m(font)->descent;
	ymin = _m(font)->descent;
	antialiasing = A;
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
	char ch = CHAR;
	XCharStruct *cs;
	if (_m(font)->per_char) {
		cs = &_m(font)->per_char[ch];
	}
	else {
		cs = &_m(defcs);
	}

	char st[2];
	st[0] = ch;
	st[1] = 0;
	car.bmp = new IPixmap;
	IPixmap::IFrame *frm= car.bmp->AddFrame();
	frm->next = frm;
	frm->width = cs->rbearing - cs->lbearing + 1;
	frm->height = cs->ascent + cs->descent;
	frm->bpp = IColorSpace::colorSpace.BytesPerPixel();
	_DWORD size = frm->width*frm->height*frm->bpp;
	frm->pic = new _BYTE[size];
	if (frm->width && frm->height) {
		Pixmap pixmap = XCreatePixmap(X11Win::display, 
									  RootWindow(X11Win::display, X11Win::screen_num), 
									  frm->width, 
									  frm->height, 
									  DefaultDepth(X11Win::display, X11Win::screen_num));
		XDrawImageString(X11Win::display, 
						 pixmap, 
						 _m(gc), 
						 0, 
						 cs->ascent, 
						 st, 
						 1);
		XImage *image = XGetImage(X11Win::display, 
								  pixmap, 
								  0, 
								  0, 
								  frm->width, 
								  frm->height, 
								  0xFFFFFF, 
								  ZPixmap);
		if (!image) {
			dprintf("IFontEngine:: XGetImage\n");
			delete car.bmp;
			car.bmp = NULL;
			return;
		}
		
		/*dprintf("char %c: %dx%d %d %d %d\n", CHAR, 
									frm->width, frm->height,
									cs->width, cs->ascent, cs->descent);*/
		/*dprintf("img: %dx%d %d %d %d\n", image->width, image->height,
									image->depth  , image->bytes_per_line,
									image->bits_per_pixel);*/
		for (_DWORD y=0; y<frm->height; y++) {
			for (_DWORD x=0; x<frm->width; x++) {
				_DWORD p = XGetPixel(image, x, y);
				if (p) {
					car.bmp->PutPixel(x, y, (*palette)[1]);
				}
				else {
					car.bmp->PutPixel(x, y, (*palette)[0]);
				}
			}
		}
		XDestroyImage(image);
		XFreePixmap(X11Win::display, pixmap);
	}
	car.xAdvance = cs->width;
	car.height = frm->height;
	car.ascent = cs->ascent;
	car.descent = cs->descent;
	car.leftbearing = 0;
}

#endif
