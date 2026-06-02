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
#ifndef _NMO_GUI_WIDGET_H_
#define _NMO_GUI_WIDGET_H_

namespace NMO {

class GWidget;
typedef void (NMOObject::*GEventFunction)(GWidget&, GEvent*);

//class GPopupMenu;

/*!
\brief		Widget super class

\author		(c) 1998-2002 Henri Michelon
\version	$Id: GWidget.hpp,v 1.2 2002/11/29 09:31:04 hmichelon Exp $*/
//---------------------------------------------------------------------
class GWidget: public GObject
{
public:
	//! Constants defining widget types
	typedef enum
	{
		WIDGET,				//! transparent widget
		PANEL,				//! rectangular widget with only a background
		BOX,				//! rectangular widget with a border and a background
		SCROLLBOX,			//! A box with scrollbars
		LINE,				//! An horizontal or vertical line
		FRAME,				//! A box with a title
		ARROW,				//! up, down, left or right directed arrows
		BUTTON,				//! rectangular button
		TOGGLEBUTTON,		//! two states rectangular button
		CHECKMARK,			//! Cross or check mark drawing
		CHECKBUTTON,		//! Button with a checkmark inside
		//RADIOBUTTON,		// two states radio box 
		TEXT,				//! single line text								
		TEXTEDIT,			//! single line text edition field
		//MEMOEDIT,			// multi lines text edition field
		UPDOWN,				//! up & down (or left & right) buttons
		SCROLLBAR,			//! scroll bar. with min, max & pos
		TRACKBAR,			//! horizontal track bar. with min, max, pos & step
		ROUNDBUTTON,		//! round button. with min, max, pos & step
		PROGRESSBAR,		//! progression bar. with min, max & pos
		PICTURE,			//! Pixmap
		LISTBOX,			//! selection list with scrollbar
		//DROPLIST,			// drop down list
		//DROPEDITLIST, 		// drop down list with a TEXTENTRY
		//MENU,
		//POPUPMENU,
		//OUTLINE,
		//TREEVIEW,
		//TOOLTIPS,
		//STATUSPANEL,
		//SPLITTER,
		GRID,
		GRIDCELL,
		TABBUTTON,			// button for TABPAGE
		TABS,
		SELECTION
	} WidgetType;

	//! Widget placement (relative to the parent widget)
	typedef enum
	{
		NONE,
		CLIENT,
		CENTER,
		HCENTER,
		VCENTER,
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		TOPCENTER,
		BOTTOMCENTER,
		LEFTCENTER,
		RIGHTCENTER,
		TOPLEFT,
		BOTTOMLEFT,
		BOTTOMRIGHT,
		TOPRIGHT,
		LEFTTOP,
		LEFTBOTTOM,
		RIGHTBOTTOM,
		RIGHTTOP,
		CORNERTOPLEFT,
		CORNERTOPRIGHT,
		CORNERBOTTOMLEFT,
		CORNERBOTTOMRIGHT
	} AlignmentType;

	//! Create a widget of a particular type
	GWidget(WidgetType = WIDGET);

	virtual ~GWidget();

	//! Return the type of the widget
	WidgetType Type() const;

	//! Return TRUE if the widget is visible
	_BOOL Visible() const;

	//! Show or hide the widget
	void Show(_BOOL = TRUE);

	//! Return TRUE is the widget is reactive to user action (mouse & keyboard)
	_BOOL Enabled() const;

	//! Enable or disable widget reactino
	void Enable(_BOOL = TRUE);

	//! Return the top border position of the widget (in pixels), relative to the parent widget
	_LONG Top() const;

	//! Return the left border position of the widget (in pixels), relative to the parent widget
	_LONG Left() const;

	/*! Move the widget to a particular position.
		\param _LONG	: Left position in pixels
		\param _LONG	: top position in pixels
		\param _BOOL	: force redraw 
	 */
	void SetPos(_LONG, _LONG, _BOOL = TRUE);

	//! Return the width of the widget, in pixels
	_DWORD Width() const;

	//! Return the height of the widget, in pixels
	_DWORD Height() const;

	/*! Resize the widget
		\param _DWORD	: width in pixels
		\param _DWORD	: height in pixels
		\param _BOOL	: force redraw
	*/	
	void SetSize(_DWORD, _DWORD, _BOOL = TRUE);

	/*! Return size size & position of the widget */
	const IRect& Rect() const;

	/*! Change the size & position of the widget
	  \param	_LONG	: left position in pixels
	  \param	_LONG	: top position in pixels
	  \param	_DWORD	: width in pixels
	  \param	_DWORD	: height in pixels
		\param _BOOL	: force redraw
	*/
	void SetRect(_LONG, _LONG, _DWORD, _DWORD, _BOOL = TRUE);

	/*! Change the size & position of the widget
	  \param	IRect	: size & position, all in pixels
	  \param	_BOOL	: force redraw
	 */ 
	void SetRect(const IRect&, _BOOL = TRUE);

	//! Return the current widget placement
	const AlignmentType Alignment() const;

	/*! Set the widget placement. Calling this method involve 
		redrawing the parent widget & resizing all the childs widgets */
	void SetAlignment(AlignmentType);

	/*! Return the current font of the widget */
	IFont& Font();

	//! Set the current font of the widget
	void SetFont(IFont&);

	//! Return TRUE if the widget have keyboard focus
	_BOOL Focused() const;

	//! Return the parent widget, or NULL */
	GWidget* Parent() const;

	/*! Return the list of direct childs widgets.
		Do NOT use this list to add or remove childs widget, 
		use Add(), Drop() & DropAll() instead */
	virtual List<GWidget>& Childs();

	/*! Add a child widget.
	  	Childs widget will NOT be destroyed on parent destruction.
		\param	GWidget	: child widget to add
		\param	AlignementType	: placement
		\param	Stringz	: resource string
		\param	_DWORD	: default padding
	*/
	virtual GWidget& Add(GWidget&, AlignmentType = NONE, const Stringz& = "", _DWORD = 0);

	/*! Add a child widget.
	  	Childs widget will be destroyed on parent destruction.
		\param	GWidget	: child widget to add
		\param	AlignementType	: placement
		\param	Stringz	: resource string
		\param	_DWORD	: default padding
	*/
	virtual GWidget* Add(GWidget*, AlignmentType = NONE, const Stringz& = "", _DWORD = 0);

	/*! Remove a child widget */
	virtual void Drop(GWidget&);

	/*! Remove all childs widgets recusivly */
	virtual void DropAll();

	/*! Change childs padding (space between childs) */
	void SetPadding(_DWORD);

	/*! Return current childs padding (space between childs) */
	_DWORD Padding() const;

	_DWORD VBorder() const;
	_DWORD HBorder() const;
	void SetVBorder(_DWORD);
	void SetHBorder(_DWORD);

	_BOOL DrawBackground() const;
	void SetDrawBackground(_BOOL);

	_BOOL& Pushed();
	_BOOL& Pointed();
	_BOOL& Freeze();
	_BOOL& Transparent();
	_BOOL& RedrawOnMouseEvent();
	_BOOL& MoveChildsOnPush();
	IRect& ChildsRect();
	void ResizeChilds();

	/*! Force a refresh of the entire widget */
	void Refresh();

	/*! Force a refresh of a part of the widget */
	void Refresh(const IRect&, _BOOL = TRUE);

	/*! Connect an object method to a event.
	  \param	EventType	: event type
	  \param	_PTR		: object address
	  \param	GEventFunction	: method offset 
	*/
	void Connect(GEvent::EventType, _PTR, const GEventFunction);

	/*! Call the object method connected to an event, if any.
	  \param	EventType	: event to simulate
	  \param	GEvent		: event parameter. Note: this pointer is AUTOMATICALLY deleted 
	*/
	void Call(GEvent::EventType, GEvent* = NULL);

	/*! Simulate a user/system event
	  \param	EventType	: event to simulate
	*/
	void Simulate(GEvent::EventType, GEvent* = NULL);

	/*GPopupMenu* PopupMenu();
	GPopupMenu* SetPopupMenu(GPopupMenu*);
	void ClosePopup();*/

	/*! Change widget resources. Use with caution ! */
	void SetResource(GResource&);

	/*! Return the user defined group index */
	_LONG GroupIndex() const;

	/*! Set the user defined group index */
	void SetGroupIndex(_LONG);

	/*! Return user data */
	_PTR Data() const;

	/*! set user data */
	void SetData(_PTR);

	friend class GWindow;

	/*! Return the parent window */
	GWindow& Window();

	/*! Return the attached display. Use with caution ! */
	IDisplay* Display();

protected:
	_DWORD			mHborder;
	_DWORD			mVborder;
	_DWORD			mPadding;
	_BOOL			focused;
	_BOOL			allowFocus;
	_BOOL			allowChilds;
	_BOOL			transparent;
	_BOOL			drawBackground;
	_BOOL			moveChildsOnPush;
	_BOOL			moveChildsNow;
	_BOOL			redrawOnMouseEvent;
	_BOOL			redrawOnMouseMove;
	_BOOL			mouseMoveOnFocus;
	IRect			rect;
	IFont			*font;
	GWidget			*parent;
	GWindow			*window;
	//GPopupMenu		*popup;
	WidgetType		type;
	AlignmentType	alignment;
	GLayout			*layout;
	GResource		*resource;

	// shortcuts
	IDisplay		*display;
	IKeyb			*keyb;

	List<GWidget>	childs;

	void MaxRect(IRect&, const IRect, const IRect) const;
	_BOOL ClipRect(IRect&, const IRect&, const IRect&) const;
	void AllowFocus(_BOOL = TRUE);

	virtual void EventCreate();
	virtual void EventDestroy();
	virtual void EventDraw(const IRect&, _BOOL = TRUE);
	virtual void EventShow();
	virtual void EventHide();
	virtual void EventEnable();
	virtual void EventDisable();
	virtual void EventMove(_LONG, _LONG, _BOOL);
	virtual void EventResize(_BOOL);
	virtual IKeyb::Key EventKeybDown(IKeyb::Key);
	virtual IKeyb::Key EventKeybUp(IKeyb::Key);
	virtual GWidget* EventMouseDown(IMouse::Button, _LONG, _LONG);
	virtual void EventMouseUp(IMouse::Button, _LONG, _LONG);
	virtual void EventMouseMove(IMouse::Button, _LONG, _LONG);
	virtual void EventGotFocus();
	virtual void EventLostFocus();

private:
	class GEventSlot
	{
	public:
		NMOObject		*obj;
		GEventFunction	func;

		GEventSlot(): obj(NULL), func(NULL) {};
	};

	_BOOL		mPushed;
	_BOOL		mPointed;
	_BOOL		mFreeze;
	_BOOL		mEnabled;
	_BOOL		mVisible;

	_PTR		mUserData;
	_LONG		mGroupIndex;
	IRect		mChildsRect;
	IRect		mRefreshRect;
	GEventSlot	slots[GEvent::nbEvents];

	GWidget* SetNextFocus();
	GWidget *SetFocus(_BOOL = TRUE);

	void FlushRefresh(IRect&);
	void ReallyDraw(const IRect&);
	void Init(GWidget&, AlignmentType, const Stringz&, _DWORD);

};

inline _LONG GWidget::Top() const { return rect.top; }
inline _LONG GWidget::Left() const { return rect.left; }
inline _DWORD GWidget::Width() const { return rect.width; }
inline _DWORD GWidget::Height() const { return rect.height; }
inline List<GWidget>& GWidget::Childs() { return childs; }
inline GWindow& GWidget::Window() { return *window; }
inline IFont& GWidget::Font() { return *font; }
inline IDisplay* GWidget::Display() { return display; }

}

#endif
