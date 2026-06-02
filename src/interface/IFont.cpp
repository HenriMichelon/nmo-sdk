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

	(c) 1998-2002 Henri Michelon

$Id: IFont.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
#include "IFontEngineFixedPixmap.hpp"
using namespace NMO;

//---------------------------------------
IFont::IFont(IFontEngine*E): engine(E)
{
	/*textColor = IRGBColor(0, 0, 0);
	bgColor = IRGBColor(255, 255, 255);
	engine->SetColors(textColor, bgColor);*/
	for (_WORD i=0; i<IFONT_CACHEBUFFERSIZE; i++) {
		cache[i].bmp = NULL;
	}
}


//---------------------------------------
IFont::~IFont()
{
	if (engine) { delete engine; }
	ResetCache();
}


//---------------------------------------
IFont* IFont::Create(const Stringz& NAME, const Stringz& ENGINE, 
					_DWORD SIZE, _BOOL B, _BOOL I, _BOOL U, _BOOL A,
					ResourcesTable *RESTABLE)
{
	IFontEngine *engine = NULL;
	if (ENGINE == "FixedPixmap") {
		engine = (IFontEngine *)new IFontEngineFixedPixmap;
	}
	else if (!ENGINE.Len()) {
		engine = new IFontEngine;
	}
	if (engine == NULL) {
		//Debug(dprintf("IFont::Create: unknown font engine %s\n", (char*)ENGINE);)
		return NULL;
	}
	engine->SetResources(RESTABLE);
	IFont *font = new IFont(engine);
	if (font->engine->OpenFont(NAME, SIZE, B, I, U, A)) {
		font->SetBgColor(IRGBColor(255, 255, 255));
		font->SetTextColor(IRGBColor(0, 0, 0));
		return font;
	}
	else {
		//Debug(dprintf("IFont::Create: Cannot open font %s\n", (char*)NAME);)
		delete font;
		return NULL;
	}
}


//---------------------------------------
void IFont::ResetCache()
{
	for (_WORD i=0; i<IFONT_CACHEBUFFERSIZE; i++)	{
		if (cache[i].bmp) {
			delete cache[i].bmp;
			cache[i].bmp = NULL;
		}
	}
}

//---------------------------------------
void IFont::SetSize(_DWORD SIZE)
{
	ASSERT(engine);
	if (engine->SetSize(SIZE)) { ResetCache(); }
}

//---------------------------------------
_DWORD IFont::Width(const UChar& CHAR)
{
	ICachedChar &font = GetFont(CHAR);
	return (font.bmp ? font.xAdvance : 0);
}


//---------------------------------------
_DWORD IFont::Height(const UChar& CHAR)
{
	ICachedChar &font = GetFont(CHAR);
	return (font.bmp ? font.height + font.descent : 0);

}


//---------------------------------------
_DWORD IFont::Width(const UStringz &STR)
{
	_DWORD w = 0;
	for (_DWORD i=0; i<STR.Len(); i++) {
		w += Width(STR[i]);

	}
	return w;
}


//---------------------------------------
_DWORD IFont::Height(const UStringz &STR)
{
	_DWORD ymax = 0;
	for (_DWORD i=0; i<STR.Len(); i++) {
		ymax = MAX(ymax, Height(STR[i]));

	}
	return ymax;
}


//---------------------------------------
IPixmap* IFont::Render(const UChar& CHAR, _LONG &XOFF, _LONG &YOFF)
{
	ICachedChar &font = GetFont(CHAR);
	XOFF = font.leftbearing;
	YOFF = font.descent;
	return font.bmp;
}


//---------------------------------------
IPixmap* IFont::Render(const UChar& CHAR)
{
	_LONG x, y;
	return Render(CHAR, x, y);
}



//---------------------------------------
void IFont::Draw(IDisplay&DISPLAY, const UChar& CHAR, _LONG X, _LONG Y, _BOOL BG)
{
	ASSERT(this);
	_LONG x, y;
	IPixmap *bmp = Render(CHAR, x, y);
	if (!bmp) return;
	if (BG)	{
		IRGBColor oldcolor = DISPLAY.PenColor();
		DISPLAY.SetPenColor(bgColor);
		DISPLAY.DrawRect(X, Y, bmp->Width(), bmp->Height());
		DISPLAY.SetPenColor(oldcolor);
	}
	bmp->Draw(DISPLAY, X+x, Y-bmp->Height()+y, !BG, TRUE);
	delete bmp;
}


//---------------------------------------
void IFont::Draw(IDisplay&DISPLAY, const UStringz &STR, _LONG X, _LONG Y, _BOOL BG)
{
	VALIDATETHIS;
	_LONG x, y;
	_LONG h = Height();
	if (BG)	{
		IRGBColor oldcolor = DISPLAY.PenColor();
		DISPLAY.SetPenColor(bgColor);
		DISPLAY.DrawRect(X, Y, Width(STR), h);
		DISPLAY.SetPenColor(oldcolor);
	}
	for (_DWORD i=0; i<STR.Len(); i++)	{
		IPixmap *bmp = Render(STR[i], x, y);
		if (!bmp) break;
		_LONG yy = Y-bmp->Height()+h+y-engine->YMin();
		bmp->Draw(DISPLAY, X+x, yy, !BG, TRUE);
		X += Width(STR[i]);
	}
}


//---------------------------------------
ICachedChar& IFont::GetFont(const UChar& CHAR)
{
	VALIDATETHIS;
	ASSERT(engine);
	ICachedChar &font = cache[_WORD(CHAR)];
	if (font.bmp) return font;
	engine->FillCache(font, CHAR);
	if (font.bmp) {
		font.bmp->SetTransColor(bgColor);
	}
	return font;
}


//---------------------------------------
void IFont::SetTextColor(const IRGBColor&C)
{
	VALIDATETHIS;
	ASSERT(engine);
	textColor.red = C.red;
	textColor.green = C.green;
	textColor.blue = C.blue;
	if (engine->SetColors(textColor, bgColor)) { ResetCache(); }
}


//---------------------------------------
void IFont::SetBgColor(const IRGBColor&C)
{
	VALIDATETHIS;
	ASSERT(engine);
	bgColor.red = C.red;
	bgColor.green = C.green;
	bgColor.blue = C.blue;
	if (engine->SetColors(textColor, bgColor)) { ResetCache(); }
}


//---------------------------------------
void IFont::Bold(_BOOL B)
{
	VALIDATETHIS;
	ASSERT(engine);
	if (engine->Bold(B)) { ResetCache(); }
}

//---------------------------------------
void IFont::Italic(_BOOL I)
{
	VALIDATETHIS;
	ASSERT(engine);
	if (engine->Italic(I)) { ResetCache(); }
}

//---------------------------------------
void IFont::Underlined(_BOOL U)
{
	VALIDATETHIS;
	ASSERT(engine);
	if (engine->Underlined(U)) { ResetCache(); }
}

