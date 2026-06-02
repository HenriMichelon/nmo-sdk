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

  TODO: make a "connect" method to implement keyboard shortcuts
 
*/
#ifndef _GUI_WINDOW_H_
#define _GUI_WINDOW_H_

namespace NMO {

/*!
\brief		A Window with a GUI

\author		(c) 1998-2002 Henri Michelon
\version	$Id: GWindow.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------*/

class GWindow: public IWindow
{
public:
	GWindow();
	virtual ~GWindow();

	/*! Return the current layout or NULL */
	GLayout* Layout() const;
	
	/*! Set the current layout. 
		If NULL, install a default layout */
	void SetLayout(GLayout*);

	/*! Set the current layout. */
	void SetLayout(GLayout&);
	
	/*! Return the main widget .
		This is widget that cover the entire window and is the parent
		of all the widgets in the window. */
	GWidget& Widget();
	
	/*! Set the main widget with optional resource string.
		Call SetLayout(NULL) if no layout have been set previously */
	GWidget& SetWidget(GWidget* = NULL, const Stringz& = "", _DWORD = 0);

	void SetFocusedWidget(GWidget*);

	/*! Display this window in a application-modal way:
		others windows can't receive user events (mouse, keyboard)
		util this window is closed. */
	_LONG ShowModal();

	/*! Event called after window creation (by the window manager) */
	inline virtual void OnCreate() {};

	/*! Event called before window destruction.
		\return TRUE if the window can be destroyed, FALSE cancel destruction */
	inline virtual _BOOL OnQueryDestroy() { return TRUE; };

	/*! Event called after window destruction (by the window manager) */
	inline virtual void OnDestroy() {};

	/*! Event called before a part of the window is refreshed */
	inline virtual void OnDraw(const IRect&) {};
	
	/*! Event called when (before) the window manager need to show the window */
	inline virtual void OnShow() {};
	
	/*! Event called when (after) the window manager need to hide the window */
	inline virtual void OnHide() {};
	
	/*! Event called after the window manager have moved the window*/
	inline virtual void OnMove() {};

	/*! Event called after the window manager have resized the window*/
	inline virtual void OnResize() {};
	
	/*! Event called when a key was pressed */
	inline virtual void OnKeybDown(IKeyb::Key) {};

	/*! Event called when a key was released */
	inline virtual void OnKeybUp(IKeyb::Key) {};
	
	/*! Event called when a mouse button was pressed */
	inline virtual void OnMouseDown(IMouse::Button, _LONG, _LONG) {};

	/*! Event called when a mouse button was released */
	inline virtual void OnMouseUp(IMouse::Button, _LONG, _LONG) {};

	/*! Event called when mouse is moved above the window */
	inline virtual void OnMouseMove(IMouse::Button, _LONG, _LONG) {};
	
	/*! Event called when the window got the keyboard focus */
	inline virtual void OnGotFocus() {};

	/*! Event called when the window lost the keyboard focus */
	inline virtual void OnLostFocus() {};

	/*! Start refresing session. All refreshed widgets will be
		recorded */
	void StartRefresh();
	
	/*! End refreshing session. All recorded widgets will be 
		refreshed, and window content updated */
	void EndRefresh();
	
	/*! Refresh the entire content of the window and all the widgets */
	void Refresh();

	/*! Exit Set the result of a ShowModal() call */
	void SetModalResult(_LONG);

private:
	_BOOL	mFreeze;
	_BOOL	mDestroylayout;
	_LONG	mModalResult;
	IRect	mRefreshrect;
	GLayout *mLayout;
	GWidget *mWidget;
	GWidget	*mFocusedWidget;

	void UnFreeze(GWidget&);

	virtual void EventCreate();
	virtual void EventDestroy();
	virtual _BOOL EventQueryDestroy();
	virtual void EventDraw(const IRect&);
	virtual void EventShow();
	virtual void EventHide();
	virtual void EventMove();
	virtual void EventResize();
	virtual void EventKeybDown(IKeyb::Key);
	virtual void EventKeybUp(IKeyb::Key);
	virtual void EventMouseDown(IMouse::Button, _LONG, _LONG);
	virtual void EventMouseUp(IMouse::Button, _LONG, _LONG);
	virtual void EventMouseMove(IMouse::Button, _LONG, _LONG);
	virtual void EventGotFocus();
	virtual void EventLostFocus();
};


}

#endif
