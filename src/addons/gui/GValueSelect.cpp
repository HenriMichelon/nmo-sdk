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

$Id: GValueSelect.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;


//--------------------------------------------------------------------------
GValueSelect::GValueSelect(WidgetType T): GPanel(T),
									value(0), step(1)
{
	min = 0;
	max = 0; 
}


//--------------------------------------------------------------------------
void GValueSelect::SetValue(_LONG V)
{
	if (value == V) { return; }
	_LONG prev = value;
	value = V;
	if (V < min) {
		value = min;
	}
	if (V > max) {
		value = max;
	}
	EventRangeChange();
	Refresh();
	if (parent) parent->Refresh(rect);
	GEventValue e(value, prev);
	Call(GEvent::OnValueChange, &e);
}


//--------------------------------------------------------------------------
void GValueSelect::SetMin(_LONG V)
{
	if (min == V) return;
	if (min > max) return;
	min = V;
	if (value < min) SetValue(min);
	ResizeChilds();
	EventRangeChange();
	Refresh();
	GEventRange e(min, max, value);
	Call(GEvent::OnRangeChange, &e);
}



//--------------------------------------------------------------------------
void GValueSelect::SetMax(_LONG V)
{
	if (max == V) return;
	if (V < min) return;
	max = V;
	if (value > max) SetValue(max);
	ResizeChilds();
	EventRangeChange();
	GEventRange e(min, max, value);
	Call(GEvent::OnRangeChange, &e);
}


//--------------------------------------------------------------------------
void GValueSelect::SetStep(_LONG V)
{
	ASSERTMSG(V != 0, "GValueSelect: can't use a step of 0");
	if (step == V) return;
	step = V;
	EventRangeChange();
	Refresh();
}


//--------------------------------------------------------------------------
_LONG GValueSelect::Value() const { return value; }


//--------------------------------------------------------------------------
_LONG GValueSelect::Min() const { return min; }


//--------------------------------------------------------------------------
_LONG GValueSelect::Max() const { return max; }


//--------------------------------------------------------------------------
_LONG GValueSelect::Step() const { return step; }


//--------------------------------------------------------------------------
void GValueSelect::SetRange(_LONG MIN, _LONG MAX, _LONG STEP) 
{
	SetStep(STEP);
	SetMax(MAX);
	SetMin(MIN);
}


//--------------------------------------------------------------------------
void GValueSelect::EventResize(_BOOL R)
{
	GWidget::EventResize(R);
	EventRangeChange();
}
