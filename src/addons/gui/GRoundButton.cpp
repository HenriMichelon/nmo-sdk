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

	(c) 2000-2001 Henri Michelon 

$Id: GRoundButton.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;

//--------------------------------------------------------------------------
GRoundButton::GRoundButton(GRoundButtonType T, _DWORD MIN, _DWORD MAX, 
					   _DWORD STEP, _DWORD VAL): 
	GValueSelect(ROUNDBUTTON), type(T), onScroll(FALSE)
{
	min = MIN;
	max = MAX;
	value = VAL;
	step = STEP;
	redrawOnMouseEvent = TRUE;
	redrawOnMouseMove = TRUE;
	mouseMoveOnFocus = TRUE;
	allowFocus = TRUE;
}


//--------------------------------------------------------------------------
GRoundButton::~GRoundButton() { }



//--------------------------------------------------------------------------
void GRoundButton::EventMouseMove(IMouse::Button B, _LONG X, _LONG Y)
{
	if (onScroll) {
		_LONG diff;
		_LONG nbvalues = max - min;
		if ((type == RB_TOP) || (type == RB_BOTTOM)) {
			diff = Y - scrollStart;
			scrollStart = Y;
			value -= diff * nbvalues / _LONG(Height() * 2);
		}
		else {
			diff = X - scrollStart;
			scrollStart = X;
			value += diff * nbvalues / _LONG(Width() * 2);
		}
		value = MIN(MAX(value, min), max);
		GEventRange e(min, max, value);
		Call(GEvent::OnValueUserChange, &e);
		Call(GEvent::OnValueChange, &e);
		Refresh();
		if (parent) { parent->Refresh(rect); }
	}
	GValueSelect::EventMouseMove(B, X, Y);
}


//--------------------------------------------------------------------------
void GRoundButton::EventMouseUp(IMouse::Button B, _LONG X, _LONG Y)
{
	if (onScroll) { onScroll = FALSE; }
	GValueSelect::EventMouseUp(B, X, Y);
}


//--------------------------------------------------------------------------
GWidget* GRoundButton::EventMouseDown(IMouse::Button B, _LONG X, _LONG Y)
{
	if ((type == RB_TOP) || (type == RB_BOTTOM)) {
		scrollStart = Y;
	}
	else {
		scrollStart = X;
	}
	onScroll = TRUE;
	return GValueSelect::EventMouseDown(B, X, Y);
}
