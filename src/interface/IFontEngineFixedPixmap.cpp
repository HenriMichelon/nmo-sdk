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
		
$Id: IFontEngineFixedPixmap.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include "IFontEngineFixedPixmap.hpp"
using namespace NMO;


//---------------------------------------
IFontEngineFixedPixmap::IFontEngineFixedPixmap(): 
	IFontEngine(), firstchar(0), nchars(0)
{
	restable = NULL;	
}


//---------------------------------------
IFontEngineFixedPixmap::~IFontEngineFixedPixmap() {}


//---------------------------------------
_BOOL IFontEngineFixedPixmap::OpenFont(const Stringz &NAME, const _DWORD, const _BOOL, 
							const _BOOL, const _BOOL, const _BOOL)
{
	Stringz name = NAME;
	if (File::FileExt(name).Len() == 0) {
		name += ".fpf";
	}
	try {
		_BOOL streamowner = FALSE;
		Stream *stream = NULL;
		if ((restable != NULL) && (name.Left(4) == "res:")) {
			Stringz fname = name.Right(name.Len() - 4);
			_LONG vpos = fname.InStr(':');
			if (vpos > -1) {
				Stringz id = fname.Left(vpos);
				id.Trim();
				fname = name.Right(fname.Len() - vpos - 1);
				fname.Trim();
				stream = restable->Get(id, fname);
				stream->Seek(sizeof(_DWORD), Stream::CURRENT);
			}			
		}
		else {
			streamowner = TRUE;
			stream = new FileStream(name);
		}
		if (stream == NULL) {
			return FALSE;
		}
		_DWORD n;
		stream->Read(&n, sizeof(n));
		firstchar = LE2DWORD(n);
		stream->Read(&n, sizeof(n));
		nchars = LE2DWORD(n);
		stream->Read(&n, sizeof(n));
		ymin = LE2DWORD(n);
		stream->Read(&n, sizeof(n));
		leftBearing = LE2DWORD(n);
		stream->Read(&n, sizeof(n));
		n = LE2DWORD(n);
		_CHAR *filetype = new _CHAR[n];
		stream->Read(filetype, n);
		height = 0;
		DatatypePixmap dtype;
		_DWORD pos, gap;
		for (_DWORD idx = 0; idx < nchars; idx ++) {
			stream->Read(&gap, sizeof(gap));
			pos = stream->Position();
			gap = LE2DWORD(gap);
			IPixmap *pxm = dtype.Load(filetype, *stream);
			if (pxm == NULL) {
				if (streamowner) { delete stream; }
				pixmaps.Clear();
				return FALSE;
			}
			pixmaps.Add(pxm);
			stream->Seek(gap - (stream->Position() - pos), Stream::CURRENT);
			height = MAX(height, pxm->Height());
		}
		antialiasing = FALSE;
		if (streamowner) { delete stream; }
		return TRUE;
	}
	catch (Stream::ExcStream) {
		return FALSE;
	}
	return FALSE;
}


//---------------------------------------
_BOOL IFontEngineFixedPixmap::SetSize(_DWORD) { return FALSE; }

//---------------------------------------
_BOOL IFontEngineFixedPixmap::Bold(const _BOOL B) { return FALSE; }

//---------------------------------------
_BOOL IFontEngineFixedPixmap::Italic(const _BOOL I) { return FALSE; }

//---------------------------------------
_BOOL IFontEngineFixedPixmap::Underlined(const _BOOL U) { return FALSE; }

//---------------------------------------
_BOOL IFontEngineFixedPixmap::SetColors(const IRGBColor &fgColor, const IRGBColor &bgColor) 
{ 
	if (palette) delete palette;
	palette = new IPalette(2);
	palette->Set(0, bgColor);
	palette->Set(1, fgColor);
	return TRUE; 
}

//---------------------------------------
void IFontEngineFixedPixmap::FillCache(ICachedChar &car, const UChar& CHAR)
{
	_WORD ccode = CHAR;
	if ((ccode < firstchar) || (ccode > (firstchar + nchars - 1))) {
		car.bmp = new IPixmap;
		if (!car.bmp) { return; }
		IPixmap::IFrame *frm= car.bmp->AddFrame();
		frm->height = height;
		frm->width = 1;
		frm->width = 1;
		frm->pic = new _BYTE[frm->width*frm->height];
		if (!frm->pic) { 
			car.bmp = NULL;
			return; 
		}
		Memory::Fill(frm->pic, 0, frm->width*frm->height);
		frm->bpp = (_BYTE)IColorSpace::colorSpace.Remap((_PTR&)frm->pic, palette, 
												frm->width*frm->height, 
												frm->bpp);
	}
	else {
		ListIterator<IPixmap> lst(pixmaps);
		car.bmp = lst[ccode - firstchar + 1].Copy();
		if (!car.bmp) { return; }
		IPixmap::IFrame *frm = car.bmp->Frame(0);
		frm->bpp = (_BYTE)IColorSpace::colorSpace.Remap((_PTR&)frm->pic,
												frm->pal, 
												frm->width*frm->height, 
												frm->bpp);
	}
	car.xAdvance = car.bmp->Width();
	car.height = car.bmp->Height() + ymin;
	car.ascent = height - car.height;
	car.descent = ymin ;
	car.leftbearing = leftBearing;
}

