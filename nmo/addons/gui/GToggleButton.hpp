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
#ifndef _NMO_GUI_TOGGLEBUTTON_H_
#define _NMO_GUI_TOGGLEBUTTON_H_

namespace NMO {

/*!
\brief		Two states button

\author		(c) 1998-2002 Henri Michelon
\version	$Id: GToggleButton.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GToggleButton: public GCheckWidget
{
public:
	GToggleButton();
	virtual ~GToggleButton() {};

protected:
	virtual GWidget* EventMouseDown(IMouse::Button, _LONG, _LONG);
};

}

#endif 
