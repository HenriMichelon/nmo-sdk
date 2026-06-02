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
#ifndef _NMO_INTERFACE_CACHEDCHAR_H_
#define _NMO_INTERFACE_CACHEDCHAR_H_

namespace NMO {

/*!
\brief		Structure used by IFont & IFontEngine to store rendered characters.

\author		(c) 1999-2001 Henri Michelon
\version	$Id: ICachedChar.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class ICachedChar: public NMOObject
{
public:
	/*! X Advance in pixels (space to the next character) */
	_DWORD	xAdvance;
	/*! left side bearing (space between left border of the char & the first char pixel) */
	_DWORD	leftbearing;
	/*! Ascent in pixel (number of pixel above the base line) */
	_DWORD	ascent; // bboxtop
	/*! Descent in pixel (number of pixel below the base line) */
	_LONG	descent; // yMin
	/*! Height in pixel (normaly ascent+descent, but only for horizontal writting) */
	_DWORD	height;
	/*! Rendered pixmap */
	IPixmap	*bmp;
};

}

#endif
