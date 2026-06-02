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
				
$Id: GUpDown.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;

#define UPDOWN_TIMER 80

//--------------------------------------------------------------------------
GUpDown::GUpDown(_DWORD MIN, _DWORD MAX, _DWORD VAL, _DWORD STEP): 
	GValueSelect(UPDOWN)
{
	transparent = TRUE;
	min = MIN;
	max = MAX;
	value = VAL;
	step = STEP;
	btnUpAlarm = new Alarm(this, Alarm::Handler(&GUpDown::OnBtnUpTimer));
	btnDownAlarm = new Alarm(this, Alarm::Handler(&GUpDown::OnBtnDownTimer));
}


//--------------------------------------------------------------------------
GUpDown::~GUpDown()
{
	delete btnDownAlarm;
	delete btnUpAlarm;
}

//--------------------------------------------------------------------------
void GUpDown::SetResources(const Stringz&RUP, const Stringz&RDOWN)
{
	switch (alignment)
	{
	case VCENTER:
		alignment = CENTER;
		break;
	case RIGHT:
		alignment = RIGHTCENTER;
		break;
	case LEFT:
		alignment = LEFTCENTER;
		break;
	default:
		break;
	}

	Add(btnUp, TOP, RUP);
	Add(btnDown, BOTTOM, RDOWN);

	btnUp.Add(new GArrow(GArrow::UP), CLIENT, RUP);
	btnDown.Add(new GArrow(GArrow::DOWN), CLIENT, RDOWN);
	
	btnUp.Connect(GEvent::OnMouseDown, this, GEventFunction(&GUpDown::OnBtnUpDown));
	btnDown.Connect(GEvent::OnMouseDown, this, GEventFunction(&GUpDown::OnBtnDownDown));
	
	AllowFocus();
}


//--------------------------------------------------------------------------
void GUpDown::EventRangeChange()
{
	btnUp.Refresh();
	btnDown.Refresh();
}


//--------------------------------------------------------------------------
void GUpDown::EventResize(_BOOL B)
{
	if (Freeze()) { return; }
	Freeze() = TRUE;
	btnUp.SetSize(0, rect.height/2);
	btnDown.SetSize(0, rect.height/2);
	Freeze() = FALSE;
	GValueSelect::EventResize(B);
}


//--------------------------------------------------------------------------
void GUpDown::EventMouseMove(IMouse::Button B, _LONG X, _LONG Y)
{
	if (!btnUp.Rect().Contains(X, Y))
		btnUpAlarm->Stop();
	if (!btnDown.Rect().Contains(X, Y))
		btnDownAlarm->Stop();
	GValueSelect::EventMouseMove(B, X, Y);
}


//--------------------------------------------------------------------------
void GUpDown::EventMouseUp(IMouse::Button B, _LONG X, _LONG Y)
{
	btnUpAlarm->Stop();
	btnDownAlarm->Stop();
	GValueSelect::EventMouseUp(B, X, Y);
}


//--------------------------------------------------------------------------
void GUpDown::OnBtnUpDown(GWidget&, GEvent*)
{
	if (value < max) {
		SetValue(value + step);
		GEventValue e(value, value);
		Call(GEvent::OnValueUserChange, &e);
		btnUpAlarm->Start(UPDOWN_TIMER);
	}
}


//--------------------------------------------------------------------------
void GUpDown::OnBtnDownDown(GWidget&, GEvent*)
{
	if (value > min) {
		SetValue(value - step);
		GEventValue e(value, value);
		Call(GEvent::OnValueUserChange, &e);
		btnDownAlarm->Start(UPDOWN_TIMER);
	}
}


//--------------------------------------------------------------------------
void GUpDown::OnBtnUpTimer(_DWORD NCALLS)
{
	if ((!Freeze()) && (NCALLS > 1))	{
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
void GUpDown::OnBtnDownTimer(_DWORD NCALLS)
{
	if ((!Freeze()) && (NCALLS > 1))	{
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
