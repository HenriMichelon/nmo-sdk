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
						
$Id: IFontEngineAmiga.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <clib/exec_protos.h>
#include <clib/graphics_protos.h>
#include <clib/diskfont_protos.h>
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;

struct Library *DiskfontBase = NULL;
_DWORD IFontEngine::nInst = 0;

//---------------------------------------
IFontEngine::IFontEngine(): palette(NULL)
{
	if (!nInst++) {
		DiskfontBase = (struct Library*)OpenLibrary("diskfont.library", 0);
	}
	if (!DiskfontBase) {
		throw NMOException("can't open diskfont.library");
	}
}


//---------------------------------------
IFontEngine::~IFontEngine()
{
	if (palette) {
		delete palette;
	}
	if (font) {
		CloseFont(font);
	}
	if (!--nInst) {
		CloseLibrary(DiskfontBase);
	}
}


//---------------------------------------
_BOOL IFontEngine::OpenFont(const Stringz &NAME, _DWORD SIZE, _BOOL B, 
						 			_BOOL I, _BOOL U, _BOOL A)
{
	TextAttr attr;
	
	if (!NAME.Len()) {
		attr.ta_Name = "courier.font";
	}
	else {
		attr.ta_Name = (char*)NAME;
	}
	
	if (SIZE) {
		attr.ta_YSize = SIZE;
	}
	else {
		attr.ta_YSize = 14;
	}
	
	attr.ta_Style = FS_NORMAL;
	if (B) {
		attr.ta_Style |= FSB_BOLD;
	}
	if (I) {
		attr.ta_Style |= FSB_ITALIC;
	}
	if (U) {
		attr.ta_Style |= FSB_UNDERLINED;
	}
	attr.ta_Flags = FPB_DISKFONT;
	
	if (!(font = OpenDiskFont(&attr))) {
		Debug(dprintf("IFontEngine: error opening font %s", attr.ta_Name);)
		return FALSE;
	}
	Debug(dprintf("IFontEngine: font %s loaded\n", attr.ta_Name);)
	height = font->tf_YSize;
	ymin = height - font->tf_Baseline;
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
void IFontEngine::FillCache(ICachedChar &cc, const UChar & CHAR)
{
	ASSERTMSG(palette, "Select foreground & background colors before printing text");
	
	char ch = CHAR;
	// Unsupported char: use the last 
	if ((ch < font->tf_LoChar) || (ch > font->tf_HiChar)) {
		ch = font->tf_HiChar+1;
	}
	
	// Extract arrays from the font struct
	typedef struct {
		UWORD offset;
		UWORD size;
	} t_fontLoc;
	
	_DWORD idx = ch - font->tf_LoChar;
	t_fontLoc *fontLoc = (t_fontLoc*)font->tf_CharLoc;
	UWORD *fontSpace = (UWORD*) font->tf_CharSpace;
	UWORD *fontKern = (UWORD*) font->tf_CharKern;
	UBYTE *fontData = (UBYTE*) font->tf_CharData;
	fontData += (fontLoc[idx].offset / 8);
	
	cc.bmp = new IPixmap;
	IPixmap::IFrame *frm= cc.bmp->AddFrame();
	frm->next = frm;
	frm->width = fontLoc[idx].size;
	frm->height = font->tf_YSize;
	frm->bpp = 1;
	_DWORD size = frm->width*frm->height;
	frm->pic = new _BYTE[size];
	Memory::Fill(frm->pic, 0, size);
	if (frm->width && frm->height) {
		_BYTE b;
		for (_DWORD y = 0; y < frm->height; y++) {
			_DWORD k = 0;
			_DWORD s = 0;
			for (_DWORD x = 0; x < frm->width; x++) {
				if (!s) {
					b = fontData[y * font->tf_Modulo + k];
					s = 128;
					if (!k++)
						s >>= (fontLoc[idx].offset-(fontLoc[idx].offset / 8)*8);
				}
				if (b & s) {
					frm->pic[y * frm->width + x] = 1;
				}
				s >>= 1;
			}	
		}
	}
	frm->pal = palette->Clone();
	cc.bmp->Remap();
	if (fontKern && fontSpace) {
		cc.xAdvance = fontSpace[idx];
	}
	else {
		cc.xAdvance = font->tf_XSize;
	}
	cc.height = font->tf_YSize;
	cc.ascent = font->tf_Baseline;
	cc.descent = cc.height - cc.ascent;
	cc.leftbearing = fontKern[idx];
}

#endif
