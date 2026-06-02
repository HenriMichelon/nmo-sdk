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
#ifndef _INTERFACE_SCREEN_H_
#define _INTERFACE_SCREEN_H_

namespace NMO {

/*!
\brief		This class describe the screen geometry

			Be careful, the screen is the application "main" 
			screen, and not the screen where the window is
			displayed.

\author		(c) 1998-2001 Henri Michelon 
\version	$Id: IScreen.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
	
class IScreen: public NMOObject
{
public:
	/*! Set size of the screen (in pixels)
		\param	_DWORD: width 
		\param	_DWORD: height
	*/
	static void SetSize(_DWORD, _DWORD);

	/*! return the width of the screen in pixels */
	static _DWORD Width();

	/*! return the height of the screen in pixels */
	static _DWORD Height(); 

private:
	static _DWORD	width;
	static _DWORD	height;
};

}

#endif
