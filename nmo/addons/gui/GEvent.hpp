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
#ifndef _NMO_GUI_EVENT_H_
#define _NMO_GUI_EVENT_H_

namespace NMO {

/*!
\brief		List of widgets events

\author		(c) 1998-2002 Henri Michelon 
\version	$Id: GEvent.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GEvent: public GObject
{
public:
	//! Event list
	typedef enum
	{
		OnCreate,		//! called after widget creation (all widgets)
		OnDestroy,		//! called before widget destruction (all widgets)
		OnDraw,			//! called after widget drawing (all widgets)
		OnKeybDown,		//! called when the user press a key & the widget have the keyboard focus (all widgets)
		OnKeybUp, 		//! called when the user press a key & the widget have the keyboard focus (all widgets)
		OnMouseDown,	//! the mouse button have been pressed above the widget or a child (all widgets)
		OnMouseUp,		//! the mouse button have been pressed above the widget or a child (all widgets)
		OnMouseMove,	//! the mouse have been moved above the widget (all widgets)
		OnResize,		//! the widget size have changed (all widgets)
		OnMove,			//! the widget position have changed (all widgets)
		OnGotFocus,		//! widget acquire keyboard focus (all widgets)
		OnLostFocus,	//! widget lost keyboard focus (all widgets)
		OnShow,			//! called after visibility change (all widgets)
		OnHide,			//! called before visibility change (all widgets)
		OnEnable,
		OnDisable,
		OnTextChange,	//! text content of the widget have changed
		OnClick,		//! the user click above the widget
		OnStateChange,	//! a CheckWidget state changed
		OnPictureChange,//! pixmap of a GPicture changed
		OnInsertItem,	//! item list of a GList widget have changed
		OnRemoveItem,	//! item list of a GList widget have changed
		OnSelectItem,	//! user selected an item list of a GList widget
		OnValueChange,	//! value of a GValueSelect widget changed
		OnValueUserChange,	//! value of a GValueSelect widget changed by the user
		OnRangeChange,	//! range of a GValueSelect widget changed
		nbEvents
	} EventType;
};


//-------------------------------------------------------
class GEventKeyb: public GEvent
{
public:
	IKeyb::Key	key;
	
	GEventKeyb(IKeyb::Key K): key(K) {};
};


//-------------------------------------------------------
class GEventMouse: public GEvent
{
public:
	IMouse::Button	button;
	_DWORD	x;
	_DWORD	y;
	
	GEventMouse(IMouse::Button B, _DWORD X, _DWORD Y): 
				button(B), x(X), y(Y) {};
	GEventMouse() {};
};


//-------------------------------------------------------
class GEventSize: public GEvent
{
public:
	_DWORD	width;
	_DWORD	height;
	
	GEventSize(_DWORD W, _DWORD H): width(W), height(H) {};
};


//-------------------------------------------------------
class GEventPos: public GEvent
{
public:
	_LONG	top;
	_LONG	left;
	
	GEventPos(_LONG T, _LONG L): top(T), left(L) {};
};


//-------------------------------------------------------
class GEventValue: public GEvent
{
public:
	_LONG	value;
	_LONG	previous;
	 
	GEventValue(_LONG V = 0, _LONG P = 0): value(V), previous(P)  {};
};


//-------------------------------------------------------
class GEventRange: public GEvent
{
public:
	_LONG	min;
	_LONG	max;
	_LONG	value;
	 
	GEventRange(_LONG I = 0, _LONG A = 0, _LONG V = 0): 
	value(V) { min = I; max = A; };
};


//-------------------------------------------------------
class GEventState: public GEvent
{
public:
	_LONG	state;
	GEventState(_LONG S = 0): state(S) {};
};


//-------------------------------------------------------
class GEventText: public GEvent
{
public:
	Stringz	text;
	GEventText(const Stringz&T = ""): text(T) {};
	virtual ~GEventText() {};
};


//-------------------------------------------------------
class GEventPicture: public GEvent
{
public:
	IPixmap	*pixmap;
};

class GWidget;

//-------------------------------------------------------
class GEventItem: public GEvent
{
public:
	_LONG	index;
	GWidget	*item;
	
	GEventItem(_LONG I, GWidget*S): index(I), item(S) {};
	virtual ~GEventItem() {};
};


}

#endif 
