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

$Id: GTrackBar.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;

//--------------------------------------------------------------------------
GTrackBar::GTrackBar(GTrackBarType T, _DWORD MIN, _DWORD MAX, 
					   _DWORD STEP, _DWORD VAL): 
	GValueSelect(TRACKBAR), type(T), onScroll(FALSE)
{
	min = MIN;
	max = MAX;
	value = VAL;
	step = STEP;
	redrawOnMouseEvent = TRUE;
	redrawOnMouseMove = TRUE;
	mouseMoveOnFocus = TRUE;
}


//--------------------------------------------------------------------------
GTrackBar::~GTrackBar() { }


//--------------------------------------------------------------------------
void GTrackBar::SetResources(const Stringz&RCAGE)
{
	Add(tracker, NONE, RCAGE);
	tracker.Connect(GEvent::OnMouseDown, this, 
					 GEventFunction(&GTrackBar::OnTrackerMouseDown));
	AllowFocus(TRUE);
	tracker.RedrawOnMouseEvent() = TRUE;
	tracker.MoveChildsOnPush() = TRUE;
}




//--------------------------------------------------------------------------
void GTrackBar::EventRangeChange()
{
	tracker.Pushed() = onScroll;
	if (rect.width && rect.height && ((max - min) != 0)) {
		_DWORD size, liftSize, liftPos;
		_DWORD nbvalues = max - min;
		if (type == VERT) {
			size = rect.height;
			liftSize = tracker.Height();
		}
		else {
			size = rect.width;
			liftSize = tracker.Width();
		}
		liftPos = _DWORD((_FLOAT(value) * (size - liftSize)) / nbvalues) - 1;
		if (type == VERT) {
			tracker.SetPos(rect.left, 
							rect.top + liftPos);
		}
		else {
			tracker.SetPos(rect.left + liftPos, 
							rect.top);
		}
	}
}

//--------------------------------------------------------------------------
void GTrackBar::EventDraw(const IRect&RECT, _BOOL R)
{
	//EventRangeChange();
	//tracker.Refresh();
	GValueSelect::EventDraw(RECT, R);
}


//--------------------------------------------------------------------------
void GTrackBar::EventResize(_BOOL B)
{
	if (Freeze()) { return; }
	Freeze() = TRUE;
	if (type == VERT) {
		tracker.SetSize(Width(), tracker.Height());
	}
	else {
		tracker.SetSize(tracker.Width(), Height());
	}
	Freeze() = FALSE;
	GValueSelect::EventResize(B);
	mouseMoveRect = IRect(rect.left - rect.width,
						  rect.top - rect.height,
						  rect.width * 3,
						  rect.height * 3);
	EventRangeChange();
}


//--------------------------------------------------------------------------
void GTrackBar::EventMouseMove(IMouse::Button B, _LONG X, _LONG Y)
{
	if (onScroll) {
		if (mouseMoveRect.Contains(X, Y)) {
			_LONG diff;
			_DWORD size;
			_DWORD nbvalues = max - min;
			if (type == VERT) {
				diff = Y - Top() ;
				size = Height() - tracker.Height();
			}
			else {
				diff = X - Left();
				size = Width() - tracker.Width();
			}
			if (diff > _LONG(scrollStart)) {
				_LONG prev = value;
				value = _LONG((_FLOAT(diff - scrollStart) * nbvalues) / size);
				value = MIN(MAX(value, min), max);
				EventRangeChange();
				GEventValue e(value, prev);
				Call(GEvent::OnValueUserChange, &e);
				Call(GEvent::OnValueChange, &e);
				if (parent) { parent->Refresh(rect); }
			}
		}
		else {
			onScroll = FALSE;
		}
	}
	GValueSelect::EventMouseMove(B, X, Y);
}


//--------------------------------------------------------------------------
void GTrackBar::EventMouseUp(IMouse::Button B, _LONG X, _LONG Y)
{
	if (onScroll) { onScroll = FALSE; }
	GValueSelect::EventMouseUp(B, X, Y);
}


//--------------------------------------------------------------------------
void GTrackBar::OnTrackerMouseDown(GWidget&, GEvent*E)
{
	onScroll = TRUE;
	if (type == VERT) {
		scrollStart = ((GEventMouse*)E)->y - tracker.Top();;
	}
	else {
		scrollStart = ((GEventMouse*)E)->x - tracker.Left();
	}
}


//--------------------------------------------------------------------------
GWidget* GTrackBar::EventMouseDown(IMouse::Button B, _LONG X, _LONG Y)
{
	_LONG longStep = step;
	_LONG diff = 0;
	if (type == VERT) {
		if (Y < tracker.Top())
			diff = -longStep;
		else if (Y > _LONG(tracker.Top() + tracker.Height()))
			diff = longStep;
		else
			return GValueSelect::EventMouseDown(B, X, Y);
	}
	else {
		if (X < tracker.Left())
			diff = -longStep;
		else if (X > _LONG(tracker.Left() + tracker.Width()))
			diff = longStep;
		else
			return GValueSelect::EventMouseDown(B, X, Y);
	}
	if (diff != 0) {
		GEventValue e(value, value);
		value = value + diff - SIGN(diff) * ((value + diff)%diff);
		value = MIN(MAX(value, min), max);
		Call(GEvent::OnValueUserChange, &e);
		Call(GEvent::OnValueChange, &e);
		EventRangeChange();
		if (parent) { parent->Refresh(rect); }
	}
	return GValueSelect::EventMouseDown(B, X, Y);
}
