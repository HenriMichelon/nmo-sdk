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
#ifndef _INTERFACE_IRGBCOL_H_
#define _INTERFACE_IRGBCOL_H_

namespace NMO {

/*!
\brief		A RGB color entry

			Note that there is not Alpha channel
			for simplification (since there are 
			rarely used in desktop applications).

\author		(c) 1998-2001 Henri Michelon 
\version	$Id: IRGBColor.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class IRGBColor: public NMOObject
{
public:
	/*!	Red component */
	_BYTE red;
	/*!	Green component */
	_BYTE green;
	/*!	Blue component */
	_BYTE blue;

	/*! Create an color with given components
		\param	_BYTE	: Red component
		\param	_BYTE	: Green component
		\param	_BYTE	: Blue component
	*/
	IRGBColor(_BYTE R = 0, _BYTE G = 0, _BYTE B = 0):
		red(R), green(G), blue(B) {};
};

}

#endif
