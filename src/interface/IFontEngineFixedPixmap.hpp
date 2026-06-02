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

*/
#ifndef _INTERFACE_FONTENGINEFIXEDPIXMAP_H_
#define _INTERFACE_FONTENGINEFIXEDPIXMAP_H_

namespace NMO {

/*!
\brief		Font rendering engine for fixed size pixmap glyphs

			This font rendering engine is intented for simples,
			easy to draw fixed width pixmaps glyphs.

\author		(c) 1999-2001 Henri Michelon
\version	$Id: IFontEngineFixedPixmap.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class IFontEngineFixedPixmap: public IFontEngine
{
public:
	IFontEngineFixedPixmap();
	virtual ~IFontEngineFixedPixmap();

	virtual _BOOL OpenFont(const Stringz&, _DWORD, _BOOL, _BOOL, _BOOL, _BOOL = FALSE);
	virtual _BOOL Bold(_BOOL);
	virtual _BOOL Italic(_BOOL);
	virtual _BOOL Underlined(_BOOL);
	virtual _BOOL SetSize(_DWORD);
	virtual _BOOL SetColors(const IRGBColor&, const IRGBColor&);
	virtual void FillCache(ICachedChar&, const UChar&);

protected:
	_DWORD			firstchar;
	_DWORD			nchars;
	_DWORD			leftBearing;
	List<IPixmap>	pixmaps;
};


}

#endif
