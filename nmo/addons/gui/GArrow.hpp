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
#ifndef _GUI_ARROW_H_
#define _GUI_ARROW_H_

namespace NMO {

/*!
\brief		simple arrow (triangular) widget

\author		(c) 1998-2002 Henri Michelon 
\version	$Id: GArrow.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GArrow: public GWidget
{
public:
	//! Direction of the arrow
	typedef enum
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	} ArrowKind;

	GArrow(ArrowKind = UP);

	//! Change arrow direction
	void SetKind(ArrowKind);

	//! Return the current arrow direction
	ArrowKind Kind() const;
	
private:
	ArrowKind kind;
};

inline GArrow::ArrowKind GArrow::Kind() const { return kind; }

}

#endif
