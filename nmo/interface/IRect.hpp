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
#ifndef _NMO_IRect_H_
#define _NMO_IRect_H_

namespace NMO {

/*!
\brief		A Rectangular zone

\author		(c) 1998-2002 Henri Michelon
\version	$Id: IRect.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class IRect: public NMOObject
{
public:
	/*! Top-Left corner (in pixels from the Top-Left of the screen)*/
	_LONG	left;
	/*! Top-Left corner (in pixels from the Top-Left of the screen)*/
	_LONG	top;
	/*! Width in pixels */
	_DWORD	width;
	/*! Height in pixels */
	_DWORD	height;

	/*! Create a rectangle with given size & position
		\param _LONG:	Left coord
		\param _LONG:	Top coord
		\param _DWORD:	width
		\param _DWORD:	height
	*/
	inline IRect(_LONG x = 0, _LONG y = 0, _DWORD w = 0, _DWORD h = 0):
		left(x), top(y), width(w), height(h) {};

	/*! Return TRUE if the given point is inside the rect */
	_BOOL Contains(_LONG X, _LONG Y) const;

	/*! Return TRUE if the given rect is inside the rect */
	_BOOL Contains(const IRect&) const;

	/*! */
	const IRect& operator = (const IRect&R);

	/*! */
	_BOOL operator == (const IRect&R) const;

	/*! The rect is the resulst of the intersection between two rects */
	void Intersection(const IRect&, const IRect&);
};

}

#endif
