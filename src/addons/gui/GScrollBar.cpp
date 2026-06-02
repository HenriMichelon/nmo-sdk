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

$Id: GScrollBar.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/addons/GUI.hpp>
#include <nmo/WindowApplication.hpp>

#define LIFT_MINWIDTH 	10
#define LONGSTEP_MUX	5
#define SCROLL_TIMER 	10

//--------------------------------------------------------------------------
GScrollBar::GScrollBar(GScrollBarType T, _DWORD MIN, _DWORD MAX,
					   _DWORD VAL, _DWORD STEP):
	GValueSelect(SCROLLBAR), type(T), onScroll(FALSE)
{
	min = MIN;
	max = MAX;
	value = VAL;
	step = STEP;
	btnUpAlarm = new Alarm(this, Alarm::Handler(&GScrollBar::OnBtnUpTimer));
	btnDownAlarm = new Alarm(this, Alarm::Handler(&GScrollBar::OnBtnDownTimer));
	liftAreaAlarm = new Alarm(this, Alarm::Handler(&GScrollBar::OnLiftAreaTimer));
	mouseMoveOnFocus = TRUE;
}


//--------------------------------------------------------------------------
GScrollBar::~GScrollBar()
{
	delete liftAreaAlarm;
	delete btnUpAlarm;
	delete btnDownAlarm;
}


//--------------------------------------------------------------------------
void GScrollBar::SetResources(const Stringz&RUP, const Stringz&RDOWN,
							  const Stringz&RAREA, const Stringz&RCAGE)
{
	Add(btnUp, NONE, RUP);
	Add(btnDown, NONE, RDOWN);
	Add(liftArea, CLIENT, RAREA);
	Add(liftCage, NONE, RCAGE);

	btnUp.Connect(GEvent::OnMouseDown, this,
				  GEventFunction(&GScrollBar::OnBtnUpDown));
	btnDown.Connect(GEvent::OnMouseDown, this,
					GEventFunction(&GScrollBar::OnBtnDownDown));
	liftArea.Connect(GEvent::OnMouseDown, this,
					 GEventFunction(&GScrollBar::OnLiftAreaDown));
	liftCage.Connect(GEvent::OnMouseDown, this,
					 GEventFunction(&GScrollBar::OnLiftCageDown));

	Align();

	AllowFocus(TRUE);
	liftCage.RedrawOnMouseEvent() = TRUE;
	liftCage.MoveChildsOnPush() = TRUE;
}


//--------------------------------------------------------------------------
void GScrollBar::Align()
{
	Freeze() = TRUE;
	if (type == VERT) {
 		btnUp.Add(new GArrow(GArrow::UP), CLIENT);
		btnDown.Add(new GArrow(GArrow::DOWN), CLIENT);
		btnUp.SetAlignment(TOP);
		btnDown.SetAlignment(BOTTOM);
	}
	else {
		btnUp.Add(new GArrow(GArrow::LEFT), CLIENT);
		btnDown.Add(new GArrow(GArrow::RIGHT), CLIENT);
		btnUp.SetAlignment(LEFT);
		btnDown.SetAlignment(RIGHT);
	}
	Freeze() = FALSE;
}


//--------------------------------------------------------------------------
void GScrollBar::EventRangeChange()
{
	liftCage.Pushed() = onScroll;
	const IRect &rect = liftArea.Rect();
	if (rect.width && rect.height && ((max - min) != 0)) {
		_DWORD size, liftSize, liftPos;
		_DWORD nbvalues = max - min;
		if (type == VERT)
			size = rect.height;
		else
			size = rect.width;
		if (size >= nbvalues) {
			liftSize = size - nbvalues;
		}
		else {
			liftSize = LIFT_MINWIDTH;
		}
		if (type == VERT) {
			liftCage.SetSize(rect.width, liftSize);
			liftSize = liftCage.Height();
		}
		else {
			liftCage.SetSize(liftSize, rect.height);
			liftSize = liftCage.Width();
		}
		liftPos = _DWORD((_FLOAT(value - min) * (size - liftSize)) / nbvalues);
		if (type == VERT) {
			liftCage.SetPos(rect.left, rect.top + liftPos);
		}
		else {
			liftCage.SetPos(rect.left + liftPos, rect.top);
		}
		liftArea.Refresh();
		liftCage.Refresh();
	}
}

//--------------------------------------------------------------------------
void GScrollBar::EventValueChange(_LONG prev)
{
	liftCage.Pushed() = onScroll;
	const IRect &rect = liftArea.Rect();
	if (rect.width && rect.height && ((max - min) != 0)) {
		_DWORD size, liftSize, liftPos;
		_DWORD nbvalues = max - min;
		if (type == VERT)
			size = rect.height;
		else
			size = rect.width;
		if (type == VERT) {
			liftSize = liftCage.Height();
		}
		else {
			liftSize = liftCage.Width();
		}
		liftPos = _DWORD((_FLOAT(value - min) * (size - liftSize)) / nbvalues);
		if (type == VERT) {
			liftCage.SetPos(rect.left, rect.top + liftPos);
		}
		else {
			liftCage.SetPos(rect.left + liftPos, rect.top);
		}
		liftCage.Refresh();
		liftArea.Refresh();
		GEventValue e(value, prev);
		Call(GEvent::OnValueUserChange, &e);
		Call(GEvent::OnValueChange, &e);
	}
}


//--------------------------------------------------------------------------
void GScrollBar::EventResize(_BOOL B)
{
	if (Freeze()) { return; }
	Freeze() = TRUE;
	_DWORD diff;
	if (type == VERT) {
		diff = rect.width;
	}
	else {
		diff = rect.height;
	}
	GValueSelect::EventResize(B);
	btnUp.SetSize(diff, diff);
	btnDown.SetSize(diff, diff);
	mouseMoveRect = IRect(rect.left - diff * 2,
						  rect.top - diff * 2,
						  rect.width + diff * 4,
						  rect.height + diff * 4);
	Freeze() = FALSE;
}


//--------------------------------------------------------------------------
void GScrollBar::EventMouseMove(IMouse::Button B, _LONG X, _LONG Y)
{
	if (onScroll) {
		if (mouseMoveRect.Contains(X, Y)) {
			_LONG diff;
			_DWORD size;
			_DWORD nbvalues = max - min;
			if (type == VERT) {
				diff = Y - liftArea.Top() ;
				size = liftArea.Height() - liftCage.Height();
			}
			else {
				diff = X - liftArea.Left();
				size = liftArea.Width() - liftCage.Width();
			}
			if (diff > _LONG(scrollStart)) {
				_LONG newval = _LONG(((diff - scrollStart) * nbvalues) / size);
				_LONG prev = value;
				value = MIN(MAX(newval, min), max);
				EventValueChange(prev);
			}
		}
		else {
			onScroll = FALSE;
		}
	}
	else {
		if (!btnUp.Rect().Contains(X, Y)) btnUpAlarm->Stop();
		if (!btnDown.Rect().Contains(X, Y)) btnDownAlarm->Stop();
		liftAreaAlarm->Stop();
	}
	GValueSelect::EventMouseMove(B, X, Y);
}


//--------------------------------------------------------------------------
void GScrollBar::EventMouseUp(IMouse::Button B, _LONG X, _LONG Y)
{
	onScroll = FALSE;
	liftAreaAlarm->Stop();
	btnUpAlarm->Stop();
	btnDownAlarm->Stop();
	GValueSelect::EventMouseUp(B, X, Y);
}


//--------------------------------------------------------------------------
void GScrollBar::OnBtnUpDown(GWidget&, GEvent*)
{
	if (value > min) {
		_LONG prev = value;
		value = MIN(MAX(value- step, min), max);
		EventValueChange(prev);
		btnUpAlarm->Start(SCROLL_TIMER);
	}
}


//--------------------------------------------------------------------------
void GScrollBar::OnBtnDownDown(GWidget&, GEvent*)
{
	if (value < max) {
		_LONG prev = value;
		value = MIN(MAX(value + step, min), max);
		EventValueChange(prev);
		btnDownAlarm->Start(SCROLL_TIMER);
	}
}


//--------------------------------------------------------------------------
void GScrollBar::OnBtnUpTimer(_DWORD NCALLS)
{
	if ((!Freeze()) && (NCALLS > 5))	{
		Freeze() = TRUE;
		_DWORD ostep = step;
		step += step * (NCALLS/10-1);
		Window().StartRefresh();
		OnBtnUpDown(*this, NULL);
		Window().EndRefresh();
		step = ostep;
		Freeze() = FALSE;
	}
}


//--------------------------------------------------------------------------
void GScrollBar::OnBtnDownTimer(_DWORD NCALLS)
{
	if ((!Freeze()) && (NCALLS > 5))	{
		Freeze() = TRUE;
		_DWORD ostep = step;
		step += step * (NCALLS/10-1);
		Window().StartRefresh();
		OnBtnDownDown(*this, NULL);
		Window().EndRefresh();
		step = ostep;
		Freeze() = FALSE;
	}
}


//--------------------------------------------------------------------------
void GScrollBar::OnLiftCageDown(GWidget&, GEvent*E)
{
	onScroll = TRUE;
	if (type == VERT) {
		scrollStart = ((GEventMouse*)E)->y - liftCage.Top();;
	}
	else {
		scrollStart = ((GEventMouse*)E)->x - liftCage.Left();
	}
}


//--------------------------------------------------------------------------
void GScrollBar::OnLiftAreaDown(GWidget&, GEvent*E)
{
	if (E != &evtMouse) { liftAreaAlarm->Stop(); }
	GEventMouse *evt = (GEventMouse*)E;
	if (liftCage.Rect().Contains(evt->x, evt->y)) { return; }
	_LONG longStep = step * LONGSTEP_MUX;
	_LONG diff = 0;
	if (type == VERT)
	{
		if (evt->y < (_DWORD)liftCage.Top())
			diff = -longStep;
		else if (evt->y > (liftCage.Top() + liftCage.Height()))
			diff = longStep;
		else
			return;
	}
	else
	{
		if (evt->x < (_DWORD)liftCage.Left())
			diff = -longStep;
		else if (evt->x > (liftCage.Left() + liftCage.Width()))
			diff = longStep;
		else
			return;
	}
	evtMouse = *evt;
	_LONG prev = value;
	value = MIN(MAX(value + diff, min), max);
	EventRangeChange();
	GEventValue e(value, prev);
	Call(GEvent::OnValueUserChange, &e);
	Call(GEvent::OnValueChange, &e);
	liftAreaAlarm->Start(SCROLL_TIMER);
}


//--------------------------------------------------------------------------
void GScrollBar::OnLiftAreaTimer(_DWORD NCALLS)
{
	if ((!Freeze()) && (NCALLS > 5))	{
		Freeze() = TRUE;
		_DWORD ostep = step;
		step += step * (NCALLS/10-1);
		Window().StartRefresh();
		OnLiftAreaDown(*this, &evtMouse);
		Window().EndRefresh();
		step = ostep;
		Freeze() = FALSE;
	}
}
