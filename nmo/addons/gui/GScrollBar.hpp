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
#ifndef _GUI_SCROLLBAR_H_
#define _GUI_SCROLLBAR_H_

namespace NMO {

/*!
\brief		horizontal or vertical scroll bar

\author		(c) 1998-2002 Henri Michelon
\version	$Id: GScrollBar.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GScrollBar: public GValueSelect
{
public:
	GButton	btnUp;
	GButton	btnDown;
	GBox	liftArea;
	GBox	liftCage;

	typedef enum
	{
		HORIZ,
		VERT
	} GScrollBarType;

	GScrollBar(GScrollBarType = HORIZ, _DWORD = 0, _DWORD = 0, _DWORD = 0, _DWORD = 1);
	virtual ~GScrollBar();
	
	GScrollBarType ScrollBarType() const;

	void SetResources(const Stringz&, const Stringz&, const Stringz&, const Stringz&);

private:
	GScrollBarType	type;
	Alarm 			*btnUpAlarm;
	Alarm 			*btnDownAlarm;
	Alarm 			*liftAreaAlarm;
	_BOOL			onScroll;
	GEventMouse		evtMouse;
	_DWORD			scrollStart;
	IRect			mouseMoveRect;

	void Align();

	virtual void EventResize(_BOOL);
	virtual void EventMouseUp(IMouse::Button, _LONG, _LONG);
	virtual void EventMouseMove(IMouse::Button, _LONG, _LONG);
	/*virtual void EventKeybDown(IKeyb::Key);
	virtual void EventKeybUp(IKeyb::Key);*/

	void OnBtnUpDown(GWidget&, GEvent*);
	void OnBtnUpTimer(_DWORD);
	void OnBtnDownDown(GWidget&, GEvent*);
	void OnBtnDownTimer(_DWORD);
	void OnLiftAreaDown(GWidget&, GEvent*);
	void OnLiftAreaTimer(_DWORD);
	void OnLiftCageDown(GWidget&, GEvent*);

	virtual void EventRangeChange();
	virtual void EventValueChange(_LONG);
};

inline GScrollBar::GScrollBarType GScrollBar::ScrollBarType() const { return type; }

class GVScrollBar: public GScrollBar
{
public:
	GVScrollBar(_DWORD I = 0, _DWORD A = 0, _DWORD V = 0, _DWORD S = 1): 
	  GScrollBar(VERT, I, A, V, S) {};
};


class GHScrollBar: public GScrollBar
{
public:
	GHScrollBar(_DWORD I = 0, _DWORD A = 0, _DWORD V = 0, _DWORD S = 1): 
	  GScrollBar(HORIZ, I, A, V, S) {};
};

}

#endif
