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
#ifndef _GUI_SELECTION_H_
#define _GUI_SELECTION_H_

namespace NMO {

/*!
\brief		Widget use for list selections

\author		(c) 1998-2002 Henri Michelon 
\version	$Id: GSelection.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GSelection: public GWidget
{
public:
	GSelection(): GWidget (SELECTION) {};
};


}

#endif
