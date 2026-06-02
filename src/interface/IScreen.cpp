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

	(c) 1998-2001 Henri Michelon 

$Id: IScreen.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;

_DWORD	IScreen::width = 0;
_DWORD	IScreen::height = 0;

void IScreen::SetSize(_DWORD W, _DWORD H)
{ 
	width = W; 
	height = H; 
}


_DWORD IScreen::Width()
{ 
	return width; 
}


_DWORD IScreen::Height()
{ 
	return height; 
}
