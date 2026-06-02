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
#ifndef _GUI_GRIDRECT_H_
#define _GUI_GRIDRECT_H_

namespace NMO {

/*!
\brief

\author		(c) 2001-2002 Henri Michelon 
\version	$Id: GGridRect.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GGridRect: public GObject
{
public:
	_DWORD top;
	_DWORD left;
	_DWORD right;
	_DWORD bottom;

	inline GGridRect (_DWORD T = 0, _DWORD L = 0, _DWORD R = 0, _DWORD B = 0):
			top(T), left(L), right(R), bottom(B) {};
};


}

#endif
