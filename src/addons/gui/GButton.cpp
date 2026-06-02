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

$Id: GButton.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/addons/GUI.hpp>


//--------------------------------------------------------------------------
GButton::GButton(): GBox(BUTTON)
{
	moveChildsOnPush = TRUE;
	redrawOnMouseEvent = TRUE;
	redrawOnMouseMove = TRUE;
	allowFocus = TRUE;
}


//--------------------------------------------------------------------------
void GButton::EventMouseUp(IMouse::Button B, _LONG X, _LONG Y)
{
	const _BOOL p = Pushed();
	GBox::EventMouseUp(B, X, Y);
	if (p && rect.Contains(X, Y)) Call(GEvent::OnClick);
}


//--------------------------------------------------------------------------
GToggleButton::GToggleButton(): GCheckWidget(TOGGLEBUTTON)
{
	moveChildsOnPush = TRUE;
	redrawOnMouseEvent = TRUE;
	allowFocus = TRUE;
}


//--------------------------------------------------------------------------
GWidget* GToggleButton::EventMouseDown(IMouse::Button B, _LONG X, _LONG Y)
{
	GWidget*r = GCheckWidget::EventMouseDown(B, X, Y);
	if (rect.Contains(X, Y)) { Call(GEvent::OnClick); }
	return r;
	
}
