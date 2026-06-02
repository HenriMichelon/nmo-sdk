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
#ifndef _INTERFACE_WINDOWS_H_
#define _INTERFACE_WINDOWS_H_

namespace NMO {

/*!
\brief			A graphical Window.

				Windows are managed by the OS/WM and
				offer minimum event handling.

				Just rewrite the EventXxxx method
				to handle an event.

				TODO: add an OnClose Event

\author			(c) 1998-2002 Henri Michelon
\version		$Id: IWindow.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------



class IWindow: public NMOObject
{
public:
	/*! Window representation type */
	typedef enum
	{
		/*! Normal (simple border), non sizeable window */
		NORMAL,
		/*! Normal window with a sizeable border */
		SIZEABLE,
		/*! Dialog box (non sizeable window, without min/max, ...) */
		DIALOG,
		/*! Window without a border and without a title */
		POPUP
	} WindowType;

	IWindow();
	virtual ~IWindow();

	/*! \return TRUE if window is currently visible */
	_BOOL Visible() const;

	/*! Show/Hide window.
		Cause an EventShow() or EventHide() event.
		\param _BOOL : action. TRUE=show, FALSE=hide
	*/
	void Show(_BOOL = TRUE);

	/*! Close & Destroy window.
		Cause an EventQuertyDestroy & an EventDestroy event */
	void Close();

	/*! Set window position.
		(top-left corner of the window area, 
		relative to the top-left of the screen)
		\param _LONG	: x position (0 based)
		\param _LONG	: y position
	*/
	void SetPos(_LONG, _LONG);

	/*! Return the y position of the client area (not the window) */
	_LONG Top() const;

	/*! Return the x position of the client area (not the window) */
	_LONG Left() const;

	/*! Return the width of the client area (not the window) */
	_DWORD Width() const;

	/*! Return the height of the client area (not the window) */
	_DWORD Height() const;

	/*! Set the size of the client area (not the window) */
	void SetSize(_DWORD, _DWORD);
	
	/*! Return the rect of the client area */
	IRect Rect() const;

	/*! Set the position & size of the client area */
	void SetRect(_LONG, _LONG, _DWORD, _DWORD);

	/*! Set the position & size of the client area */
	void SetRect(const IRect&);

	/*! Center the window on the screen */
	void ScreenCenter();

	/*! Return TRUE if window have the keyboard focus */
	_BOOL HasFocus() const;

	/*! Give the keyboard focus to the window */
	void GiveFocus();

	/*! Return the title of the window */
	UStringz Title() const;

	/*! Change the window default background color */
	void SetBgColor (IRGBColor&);

	/*! Set the title of the window */
	void SetTitle(const UStringz&);

	/*! Called on window creation (during WindowApplication::Add()) */
	virtual void EventCreate();
	/*! Called on window destruction, after EventQueryDestroy() */
	virtual void EventDestroy();
	/*! Called before EventDestroy.
		Destruction is done onlyTriggered only if TRUE is returned.
		Warning: called only on system destroy, not on Close() */
	virtual _BOOL EventQueryDestroy() { return TRUE; };
	/*! Called is widow need to be redrawed, before display refresh
		\param	IRect	: zone to be refreshed
	*/
	virtual void EventDraw(const IRect&);
	/*! Called on (before) mapping */
	virtual void EventShow();
	/*! Called on (after) unmapping */
	virtual void EventHide();
	/*! Called on position modification */
	virtual void EventMove();
	/*! Called on size modification */
	virtual void EventResize();
	/*! Called when a key if pressed.
		\param	IKeyb::Key	: key code
	*/
	virtual void EventKeybDown(IKeyb::Key);
	/*! Called when a key if released
		\param	IKeyb::Key	: key code
	*/
	virtual void EventKeybUp(IKeyb::Key);
	/*! Called if a mouse button is pressed
		\param	IMouse::Button	: button code
		\param	_LONG	: mouse X position
		\param	_LONG	: mouse Y position
	*/
	virtual void EventMouseDown(IMouse::Button, _LONG, _LONG);
	/*! Called if a mouse button is release
		\param	IMouse::Button	: button code
		\param	_LONG	: mouse X position
		\param	_LONG	: mouse Y position
	*/
	virtual void EventMouseUp(IMouse::Button, _LONG, _LONG);
	/*! Called if a mouse is moved
		\param	IMouse::Button	: pressed button code (if any)
		\param	_LONG	: mouse X position
		\param	_LONG	: mouse Y position
	*/
	virtual void EventMouseMove(IMouse::Button, _LONG, _LONG);
	/*! Called when the window get the keyboard focus */
	virtual void EventGotFocus();
	/*! Called when the window lost the keyboard focus */
	virtual void EventLostFocus();

	/*! Return the attached keyboard */
	IKeyb& Keyb();

	/*! Return the attached display device */
	IDisplay& Display();

	/*! Return the architecture dependent handler */
	IHandle Handle() const;

	/*! Force rendering of the display device for a part of the window */
	void RefreshDisplay(const IRect&);

	/*! Regenerate display device to match the window size */
	void ResizeDisplay();


	class Link
	{
	public:
		IHandle handle;
		IWindow	*nmo;
		_BOOL	dynamic;

		static List<Link> handles;
		static IWindow* Handle2Nmo(const IHandle&);
		static IHandle Nmo2Handle(const IWindow&);
		static void Drop(const IWindow&);

		inline Link(IWindow*O, _BOOL D): nmo(O), dynamic(D) {};
		inline ~Link() { if (dynamic) delete nmo; };
	};

#ifdef HAVE_X11
	IRect	oldRect;
	_BOOL	closing;
#endif
protected:
	IKeyb		keyb;
	IDisplay	*display;
	IHandle		handle;

private:
// this fields needs to be replaced with the abstract field !!

#ifdef _AMIGA
	WindowType type;
	Link		  *link;
	IRect		  newRect;
	Stringz	  title;
	_BOOL		  active;
#else
	_PTR mIWindowAbstract;
#endif

#ifdef _QNX
	_PTR	raw;
#endif

#ifdef _WIN32
	_DWORD	style;
#endif

	friend class WindowApplication;
};

inline IDisplay& IWindow::Display() { return *display; };
inline IKeyb& IWindow::Keyb() { return keyb; };
inline IHandle IWindow::Handle() const { return handle; };

inline void IWindow::EventCreate() {};
inline void IWindow::EventDestroy() { };
inline void IWindow::EventDraw(const IRect&) {};
inline void IWindow::EventResize() {};
inline void IWindow::EventShow() {};
inline void IWindow::EventHide() {};
inline void IWindow::EventMove() {};
inline void IWindow::EventKeybDown(IKeyb::Key) {};
inline void IWindow::EventKeybUp(IKeyb::Key) {};
inline void IWindow::EventMouseDown(IMouse::Button, _LONG, _LONG) {};
inline void IWindow::EventMouseUp(IMouse::Button, _LONG, _LONG) {};
inline void IWindow::EventMouseMove(IMouse::Button, _LONG, _LONG) {};
inline void IWindow::EventGotFocus() {};
inline void IWindow::EventLostFocus() {};

}

#endif
