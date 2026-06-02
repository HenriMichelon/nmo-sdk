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

	(c) 1998-2002 Henri Michelon

$Id: GWidget.cpp,v 1.2 2002/11/29 13:27:52 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;


//----------------------------------------------------------
GWidget::GWidget(WidgetType T): focused(FALSE),
								allowFocus(FALSE), allowChilds(TRUE),
								drawBackground(TRUE),
								moveChildsOnPush(FALSE),
								redrawOnMouseEvent(FALSE),
								redrawOnMouseMove(FALSE),
								mouseMoveOnFocus(FALSE),
								font(NULL),
								parent(NULL), window(NULL),
								//popup(NULL),
								type(T),
								alignment(NONE),
								layout(NULL),
								display(NULL), keyb(NULL)
{
	transparent = (type == WIDGET);
	mHborder = mVborder = mPadding = mGroupIndex = 0;
	mUserData = resource = NULL;
	moveChildsNow = mPushed = mPointed = FALSE;
	mVisible = mFreeze = mEnabled = TRUE;
}


//----------------------------------------------------------
GWidget::~GWidget()
{
	/*if (parent) {
		parent->Drop(*this);
	}*/
	if (focused && window) {
		window->SetFocusedWidget(NULL);
	}
	if (resource) {
		delete resource;
		//layout->DeleteResource(resource);
	}
	//if (popup) delete popup;
}


//----------------------------------------------------------
_BOOL GWidget::Visible() const
{
	const GWidget *p = this;
	do {
		if (!p->mVisible) { return FALSE; }
	}
	while ( (p = p->parent) != NULL);
	return (window && window->Visible());
}


//----------------------------------------------------------
void GWidget::Show(_BOOL S)
{
	if (mVisible == S) return;
	mVisible = S;
	if (mVisible) {
		EventShow();
	}
	else {
		EventHide();
	}
}


//----------------------------------------------------------
void GWidget::Enable(_BOOL S)
{
	if (mEnabled == S) return;
	mEnabled = S;
	if (mEnabled) {
		EventEnable();
	}
	else {
		EventDisable();
	}
}


//----------------------------------------------------------
void GWidget::SetPos(_LONG L, _LONG T, _BOOL REDRAW)
{
	if ((L == rect.left) && (T == rect.top)) return;
	EventMove(L, T, REDRAW);
}


//----------------------------------------------------------
void GWidget::SetSize(_DWORD W, _DWORD H, _BOOL REDRAW)
{
	if ((W == rect.width) && (H == rect.height)) return;
	if (parent) { parent->Refresh(rect); }
	rect.width = W;
	rect.height = H;
	EventResize(REDRAW);
}


 //----------------------------------------------------------
GWidget* GWidget::SetNextFocus()
{
	if (focused) {
		SetFocus(FALSE);
	}
	else {
		GWidget *r = SetFocus();
		if (r) return r;
	}

	if (!parent) return NULL;
	_DWORD idx;
	GWidget *p = parent;
	GWidget *s = this;
	ListIterator<GWidget> list;
	do {
		list = p->childs;
		if (!((idx = list.IndexOf(*s)) == p->childs.Count())) { break; }
		s = p;
		p = p->parent;
		if (!p) return s->SetFocus();
	} while (TRUE);
	return list[idx+1].SetNextFocus();
}


 //----------------------------------------------------------
GWidget* GWidget::SetFocus(_BOOL F)
{
	if (!mEnabled) return NULL;

	if (F && (!allowFocus)) {
		ListIterator<GWidget> list(childs);
		while (!list.End()) {
			GWidget *w = list.Next().SetFocus(F);
			if (w) return w;
		}
		return NULL;
	}

	if (focused != F) {
		focused = F;
		if (F) {
			if (!mFreeze) { Refresh(); }
			Call(GEvent::OnGotFocus);
		}
		else {
			Call(GEvent::OnLostFocus);
			/*GWidget *p = parent;
			while (p && (!p->DrawBackground())) p = p->parent;
			if (p) { p->Refresh(rect); }*/
		}
	}
	return this;
}


//----------------------------------------------------------
void GWidget::AllowFocus(_BOOL A)
{
	allowFocus = A;
	ListIterator<GWidget> list(childs);
	while (!list.End()) {
		list.Next().AllowFocus(FALSE);
	}
}


//----------------------------------------------------------
void GWidget::Init(GWidget&WND, AlignmentType ALIGN,
				   const Stringz&RES, _DWORD P)
{
	ASSERT(display);
	ASSERT(keyb);
	ASSERT(window);
	WND.mPadding = P;
	WND.alignment = ALIGN;
	WND.display = display;
	WND.keyb = keyb;
	if (!WND.font) { WND.font = font; }
	WND.window = window;
	WND.layout = layout;
	WND.parent = this;
	layout->AddResource(WND, RES);
	WND.EventCreate();
	if (window->Visible() && resource) { ResizeChilds(); }
	WND.mFreeze = FALSE;
	//dprintf("new widget: %x, type=%d\n", &WND, WND.Type());
}


//----------------------------------------------------------
void GWidget::Drop(GWidget&W)
{
	ListIterator<GWidget> list(childs);
	_DWORD idx = list.IndexOf(W);
	if (idx) {
		/*dprintf("drop %x, %x, %x\n", this, &W, parent);*/
		W.parent = NULL;
		list = W.Childs();
		while (!list.End()) {
			W.Drop(list.Next());
		}
		childs.Drop(idx);
		ResizeChilds();
	}
	Refresh();
}


//----------------------------------------------------------
void GWidget::DropAll()
{
	ListIterator<GWidget> list(childs);
	while (!list.End()) {
		list.Next().DropAll();
	}
	childs.Clear();
	Refresh();
}


//----------------------------------------------------------
GWidget& GWidget::Add(GWidget&WND, AlignmentType ALIGN, const Stringz&RES,
					  _DWORD P)
{
	PRE(window, "GWidget::Add: widget must be added to another widget before use");
	if (!allowChilds) return WND;
	childs.Add(WND);
	Init(WND, ALIGN, RES, P);
	return WND;
}


//----------------------------------------------------------
GWidget* GWidget::Add(GWidget*WND, AlignmentType ALIGN, const Stringz&RES,
					  _DWORD P)
{
	PRE(window, "GWidget::Add: widget must be added to another widget before use");
	PRE(WND, "NULL Widget");
	if (!allowChilds) return WND;
	childs.Add(WND);
	Init(*WND, ALIGN, RES, P);
	return WND;
}


//----------------------------------------------------------
void GWidget::Connect(GEvent::EventType TYP, _PTR OBJ,
					  const GEventFunction FUNC)
{
	PRE(OBJ, "Invalid object for event slot connection");
	PRE(FUNC, "Invalid method for event slot connection");
	slots[TYP].obj = (NMOObject*)OBJ;
	slots[TYP].func = FUNC;
}


//----------------------------------------------------------
void GWidget::Simulate(GEvent::EventType TYP, GEvent *EVT)
{
	switch (TYP) {
	case GEvent::OnClick:
		EventMouseDown(IMouse::LEFT, Rect().left, Rect().top);
		EventMouseUp(IMouse::LEFT, Rect().left, Rect().top);
		break;
	default:
		Call(TYP, EVT);
		break;
	}
}


//----------------------------------------------------------
void GWidget::Call(GEvent::EventType TYP, GEvent *EVT)
{
	const GWidget::GEventSlot &slot = slots[TYP];
	if (slot.func) {
		ASSERT(slot.obj);
		(slot.obj->*slot.func)(*this, EVT);
	}
}


//----------------------------------------------------------
void GWidget::EventCreate()
{
	Call(GEvent::OnCreate);
}


//----------------------------------------------------------
void GWidget::EventDestroy()
{
	ListIterator<GWidget> list(childs);
	while (!list.End()) {
		list.Next().EventDestroy();
	}
	//if (popup) popup->EventDestroy();
	Call(GEvent::OnDestroy);
}


//----------------------------------------------------------
void GWidget::EventShow()
{
	if (mVisible) {
		Call(GEvent::OnShow);
		ListIterator<GWidget> list(childs);
		while (!list.End()) {
			list.Next().EventShow();
		}
		if (parent) { Refresh(); }
	}
}


//----------------------------------------------------------
void GWidget::EventHide()
{
	if (!mVisible) {
		ListIterator<GWidget> list(childs);
		while (!list.End()) {
			list.Next().EventHide();
		}
		if (parent) { parent->Refresh(rect); }
		Call(GEvent::OnHide);
	}
}


//----------------------------------------------------------
void GWidget::EventEnable()
{
	Call(GEvent::OnEnable);
	ListIterator<GWidget> list(childs);
	while (!list.End()) {
		list.Next().Enable();
	}
	Refresh();
}


//----------------------------------------------------------
void GWidget::EventDisable()
{
	ListIterator<GWidget> list(childs);
	while (!list.End()) {
		list.Next().Enable(FALSE);
	}
	Call(GEvent::OnDisable);
	Refresh();
}


//----------------------------------------------------------
void GWidget::EventMove(_LONG L, _LONG T, _BOOL REDRAW)
{
	IRect old_rect(rect);
	_LONG diffX = rect.left - L;
	_LONG diffY = rect.top - T;
	rect.left = L;
	rect.top = T;
	ListIterator<GWidget> list(childs);
	while (!list.End()) {
		GWidget &w = list.Next();
		w.SetPos(w.rect.left - diffX, w.rect.top - diffY, FALSE);
	}
	GEventPos event(L, T);
	Call(GEvent::OnMove, &event);
	if (parent) { parent->Refresh(old_rect); }
	if (REDRAW) { Refresh(); }
}


//----------------------------------------------------------
void GWidget::EventResize(_BOOL REDRAW)
{
	if (mFreeze) return;
	mFreeze = TRUE;
	if (REDRAW) { Refresh(); }
	if (parent && (!parent->mFreeze)) parent->ResizeChilds();
	if (rect.width && rect.height) {
		GEventSize event(rect.width, rect.height);
		Call(GEvent::OnResize, &event);
		ResizeChilds();
	}
	mFreeze = FALSE;
}


//----------------------------------------------------------
void GWidget::ResizeChilds()
{
	if (!layout) { return; }
	mFreeze = TRUE;
	layout->Resize(*this, rect, *resource);
	IRect arect = rect;
	arect.left += mHborder + mPadding + mChildsRect.left;
	if (arect.width > (2*mHborder + 2*mPadding) ) {
		arect.width -= 2*mHborder + 2*mPadding;
	}
	else {
		arect.width = 0;
	}
	arect.top += mVborder + mPadding + mChildsRect.top;
	if (arect.height > (2*mVborder + 2*mPadding) ) {
		arect.height -= 2*mVborder + 2*mPadding;
	}
	else {
		arect.height = 0;
	}

	ListIterator<GWidget> list(childs);
	while ((arect.width>0) && (arect.height>0) && (!list.End())) {
		GWidget &w = list.Next();
		IRect wrect = w.rect;
		/*dprintf("ResizeChild: %d,%d,%d,%d / %d,%d,%d,%d\n",
				wrect.left, wrect.top, wrect.width, wrect.height,
				arect.left, arect.top, arect.width, arect.height);*/
		switch (w.alignment)
		{
		case CLIENT:
			wrect = arect;
			arect.width = 0;
			arect.height = 0;
			break;
		case CENTER:
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + (arect.width - wrect.width)/2;
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + (arect.height- wrect.height)/2;
			arect.width = 0;
			arect.height = 0;
			break;
		case VCENTER:
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + (arect.width - wrect.width)/2;
			wrect.top = arect.top;
			wrect.height = arect.height;
			arect.width = 0;
			break;
		case HCENTER:
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + (arect.height- wrect.height)/2;
			wrect.left = arect.left;
			wrect.width = arect.width;
			arect.width = 0;
			break;
		case TOP:
			wrect.left = arect.left;
			wrect.top = arect.top;
			wrect.width = arect.width;
			arect.top += wrect.height + mPadding;
			//arect.top = MIN(rect.height, arect.top + wrect.height + mPadding);
			arect.height = MAX(0l, _LONG(arect.height - (wrect.height + 2*mPadding)));
			break;
		case LEFT:
			wrect.left = arect.left;
			wrect.top = arect.top;
			wrect.height = arect.height;
			arect.left += wrect.width + mPadding;
			//arect.left = MIN(rect.width, arect.left + wrect.width + mPadding);
			arect.width = MAX(0l, _LONG(arect.width - (wrect.width + 2*mPadding)));
			break;
		case BOTTOM:
			wrect.left = arect.left;
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + arect.height - wrect.height;
			arect.height = MAX(0l, _LONG(arect.height - (wrect.height + 2*mPadding)));
			wrect.width = arect.width;
			break;
		case RIGHT:
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + arect.width - wrect.width;
			wrect.top = arect.top;
			wrect.height = arect.height;
			arect.width = MAX(0l, _LONG(arect.width - (wrect.width + 2*mPadding)));
			break;
		case TOPCENTER:
			wrect.top = arect.top;
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + (arect.width - wrect.width)/2;
			arect.top += wrect.height + mPadding;
			//arect.top = MIN(rect.height, arect.top + wrect.height + mPadding);
			arect.height -= wrect.height + 2*mPadding;
			//arect.height = MAX(0l, _LONG(arect.height - (wrect.height + 2*mPadding)));
			break;
		case BOTTOMCENTER:
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + arect.height - wrect.height;
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + (arect.width - wrect.width)/2;
			arect.height = MAX(0l, _LONG(arect.height - (wrect.height + 2*mPadding)));
			break;
		case LEFTCENTER:
			wrect.left = arect.left;
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + (arect.height- wrect.height)/2;
			arect.left += wrect.width + mPadding;
			//arect.left = MIN(rect.width, arect.left + wrect.width + mPadding);
			arect.width = MAX(0l, _LONG(arect.width - (wrect.width + 2*mPadding)));
			break;
		case RIGHTCENTER:
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + arect.width - wrect.width;
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + (arect.height- wrect.height)/2;
			arect.width = MAX(0l, _LONG(arect.width - (wrect.width + 2*mPadding)));
			break;
		case TOPLEFT:
			wrect.left = arect.left;
			wrect.top = arect.top;
			arect.top += wrect.height + mPadding;
			//arect.top = MIN(rect.height, arect.top + wrect.height + mPadding);
			arect.height -= wrect.height + 2*mPadding;
			//arect.height = MAX(0l, _LONG(arect.height - (wrect.height + 2*mPadding)));
			break;
		case BOTTOMLEFT:
			wrect.left = arect.left;
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + arect.height - wrect.height;
			arect.height = MAX(0l, _LONG(arect.height - (wrect.height + 2*mPadding)));
			break;
		case BOTTOMRIGHT:
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + arect.width - wrect.width;
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + arect.height - wrect.height;
			arect.height = MAX(0l, _LONG(arect.height - (wrect.height + 2*mPadding)));
			break;
		case TOPRIGHT:
			wrect.top = arect.top;
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + arect.width - wrect.width;
			arect.height -= wrect.height + 2*mPadding;
			//arect.height = MAX(0l, _LONG(arect.height - (wrect.height + 2*mPadding)));
			arect.top += wrect.height + mPadding;
			//arect.top = MIN(rect.height, arect.top + wrect.height + mPadding);
			break;
		case LEFTTOP:
			wrect.left = arect.left;
			wrect.top = arect.top;
			arect.left += wrect.width + mPadding;
			//arect.left = MIN(rect.width, arect.left + wrect.width + mPadding);
			arect.width = MAX(0l, _LONG(arect.width - (wrect.width + 2*mPadding)));
			break;
		case LEFTBOTTOM:
			wrect.left = arect.left;
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + arect.height - wrect.height;
			arect.left += wrect.width + mPadding;
			//arect.left = MIN(rect.width, arect.left + wrect.width + mPadding);
			arect.width = MAX(0l, _LONG(arect.width - (wrect.width + 2*mPadding)));
			break;
		case RIGHTBOTTOM:
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + arect.width - wrect.width;
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + arect.height - wrect.height;
			arect.width = MAX(0l, _LONG(arect.width - (wrect.width + 2*mPadding)));
			break;
		case RIGHTTOP:
			wrect.top = arect.top;
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + arect.width - wrect.width;
			arect.width = MAX(0l, _LONG(arect.width - (wrect.width + 2*mPadding)));
			break;
		case CORNERTOPLEFT:
			wrect.left = arect.left;
			wrect.top = arect.top;
			arect.top += wrect.height + mPadding;
			break;
		case CORNERBOTTOMLEFT:
			wrect.left = arect.left;
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + arect.height - wrect.height;
			break;
		case CORNERBOTTOMRIGHT:
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + arect.width - wrect.width;
			if (wrect.height > arect.height)
				wrect.top = 0;
			else
				wrect.top = arect.top + arect.height - wrect.height;
			break;
		case CORNERTOPRIGHT:
			wrect.top = arect.top;
			if (wrect.width > arect.width)
				wrect.left = 0;
			else
				wrect.left = arect.left + arect.width - wrect.width;
			arect.top += wrect.height + mPadding;
			break;
		default:
			continue;
			break;
		}
		/*dprintf("ResizeChild: %d,%d,%d,%d / %d,%d,%d,%d\n",
				wrect.left, wrect.top, wrect.width, wrect.height,
				arect.left, arect.top, arect.width, arect.height);*/
		w.rect = wrect;
		w.EventResize(FALSE);
	}
	mFreeze = FALSE;
}


//----------------------------------------------------------
IKeyb::Key GWidget::EventKeybDown(IKeyb::Key K)
{
	PRE(mEnabled, "Disabled widget received a KeybUp event");
	GEventKeyb event(K);
	Call(GEvent::OnKeybDown, &event);
	return event.key;
}


//----------------------------------------------------------
IKeyb::Key GWidget::EventKeybUp(IKeyb::Key K)
{
	PRE(mEnabled, "Disabled widget received a KeybUp event");
	if (focused) {
		GEventKeyb event(K);
		Call(GEvent::OnKeybUp, &event);
		return event.key;
	}
	return K;
}


//----------------------------------------------------------
/*void GWidget::ClosePopup()
{
	if (popup) popup->Show(FALSE);
	ListIterator<GWidget> list(childs);
	while (!list.End())  { list.Next().ClosePopup(); }
}*/


//----------------------------------------------------------
GWidget* GWidget::EventMouseDown(IMouse::Button B, _LONG X, _LONG Y)
{
	if (!mEnabled) return NULL;

	/*_DWORD close = popup && popup->Visible();
	ClosePopup();*/

	mPushed = !Transparent();
	GWidget *wfocus = NULL;

	ListIterator<GWidget> list(childs);
	while (!list.End()) {
		GWidget &w = list.Next();
		if (w.Rect().Contains(X, Y)) {
			wfocus = w.EventMouseDown(B, X, Y);
			if ((!wfocus) && w.allowFocus) { wfocus = w.SetFocus(); }
			if (w.redrawOnMouseEvent && w.Rect().Contains(X, Y) && (!w.Transparent())) {
				w.Refresh();
			}
		}
	}
	if (redrawOnMouseEvent && rect.Contains(X, Y) && (!transparent)) { Refresh(); }

	/*if (popup && (B == IMouse::RIGHT) && (!wfocus) && (!close) && popup->AutoPopup()) {
		popup->SetPos(IMouse::XPos(), IMouse::YPos());
		popup->Show(TRUE);
		return NULL;
	}*/

	GEventMouse event(B, X, Y);
	Call(GEvent::OnMouseDown, &event);
	return wfocus;
}


//----------------------------------------------------------
void GWidget::EventMouseUp(IMouse::Button B, _LONG X, _LONG Y)
{
	if (!mEnabled) { return; }
	_BOOL r = redrawOnMouseEvent && (rect.Contains(X, Y) || mPushed);
	mPushed = FALSE;
	ListIterator<GWidget> list(childs);
	while (!list.End()) {
		GWidget &w = list.Next();
		w.EventMouseUp(B, X, Y);
		if (w.redrawOnMouseEvent && (w.Rect().Contains(X, Y) || w.mPushed) && (!w.Transparent())) {
			w.Refresh();
		}
	}
	if (r && (!transparent)) { Refresh(); }
	GEventMouse event(B, X, Y);
	Call(GEvent::OnMouseUp, &event);
}


//----------------------------------------------------------
void GWidget::EventMouseMove(IMouse::Button B, _LONG X, _LONG Y)
{
	if (!mEnabled) { return; }
	_BOOL p = rect.Contains(X, Y);
	ListIterator<GWidget> list(childs);
	while (!list.End()) {
		GWidget &w = list.Next();
		p = w.Rect().Contains(X, Y);
		if (w.redrawOnMouseMove && (w.mPointed != p)) {
			w.mPointed = p;
			w.Refresh();
		}
		if (p) {
			w.EventMouseMove(B, X, Y);
		} else if (w.mPushed) {
			w.EventMouseUp(B, X, Y);
		}
	}
	if (redrawOnMouseMove && (mPointed != p) && (!transparent)) { Refresh(); }
	GEventMouse event(B, X, Y);
	Call(GEvent::OnMove, &event);
}



//----------------------------------------------------------
void GWidget::EventGotFocus()
{
	Call(GEvent::OnGotFocus);
}


//----------------------------------------------------------
void GWidget::EventLostFocus()
{
	Call(GEvent::OnLostFocus);
}


//----------------------------------------------------------
_BOOL GWidget::ClipRect(IRect&R, const IRect& R1, const IRect& R2) const
{
	if ((!R1.width) || (!R1.height) ||
		(!R2.width) || (!R2.height)) {
		return FALSE;
	}
	if (R1.left > R2.left) {
		R.width = MIN(R1.width, R2.width - (R1.left - R2.left));
		R.left = R1.left;
	}
	else {
		R.width = MIN(R2.width, R1.width - (R2.left - R1.left));
		R.left = R2.left;
	}
	if (R1.top > R2.top) {
		R.height = MIN(R1.height, R2.height - (R1.top - R2.top));
		R.top = R1.top;
	}
	else {
		R.height = MIN(R2.height, R1.height - (R2.top - R1.top));
		R.top = R2.top;
	}
	return ((R.width > 0) && (R.height > 0));
}


//----------------------------------------------------------
void GWidget::MaxRect(IRect&R, const IRect A, const IRect B) const
{
	if ((!A.width) || (!A.height)) {
		R = B;
	}
	else if ((!B.width) || (!B.height)) {
		R = A;
	}
	else {
		R.width = MAX(0, MAX(A.left + _LONG(A.width), B.left + _LONG(B.width)));
		R.height = MAX(0, MAX(A.top + _LONG(A.height), B.top + _LONG(B.height)));
		R.left = MAX(0, MIN(A.left, B.left));
		R.top = MAX(0, MIN(A.top, B.top));
		R.width -= R.left;
		R.height -= R.top;
	}
	/*dprintf("MaxRect: %d,%d,%d,%d / %d,%d,%d,%d = %d,%d,%d,%d\n",
		A.left, A.top, A.width, A.height,
		B.left, B.top, B.width, B.height,
		R.left, R.top, R.width, R.height);*/
}


//----------------------------------------------------------
void GWidget::SetPadding(_DWORD P)
{
	mPadding = P;
	EventResize(TRUE);
}


//----------------------------------------------------------
void GWidget::SetDrawBackground(_BOOL D)
{
	drawBackground = D;
	Refresh();
}


//----------------------------------------------------------
void GWidget::SetAlignment(GWidget::AlignmentType ALIGN)
{
	alignment = ALIGN;
	EventResize(TRUE);
}


//----------------------------------------------------------
void GWidget::Refresh()
{
	if (!mFreeze) {
		Refresh(rect);
	}
}


//----------------------------------------------------------
void GWidget::Refresh(const IRect&R, _BOOL PARENT)
{
	if ((!mVisible) || (window && (!window->Visible())) ||
		(!R.width) || (!R.height) || (!rect.width) || (!rect.height)) {
		return;
	}
	if (PARENT && (transparent || (!mVisible)) && parent) {
		parent->Refresh(R);
	}
	IRect refreshzone;
	if (ClipRect(refreshzone, rect, R)) {
		GWidget *p = parent;
		while (p) {
			if (!ClipRect(refreshzone, refreshzone, p->rect)) {
				//Debug(dprintf("EventDraw(%x): refreshzone out of parent widget rect\n", this));
				return;
			}
			p = p->parent;
		}
		MaxRect(mRefreshRect, mRefreshRect, refreshzone);
		/*Debug(dprintf("Refresh(%x, %d): %d,%d,%d,%d -> %d,%d,%d,%d\n", this, Type(),
				R.left, R.top, R.width, R.height,
				mRefreshRect.left, mRefreshRect.top, mRefreshRect.width, mRefreshRect.height	);)*/
	}
}


//----------------------------------------------------------
void GWidget::FlushRefresh(IRect&R)
{
	if (mRefreshRect.width && mRefreshRect.height) {
		MaxRect(R, R, mRefreshRect);
		ReallyDraw(mRefreshRect);
	}
	ListIterator<GWidget> list(childs);
	while (!list.End()) {
		list.Next().FlushRefresh(R);
	}
}


//----------------------------------------------------------
void GWidget::ReallyDraw(const IRect &R)
{
	IRect refreshzone;
	if (!ClipRect(refreshzone, rect, R)) {
		//Debug(dprintf("ReallyDraw(%x): refreshzone out of widget rect\n", this));
		mRefreshRect.top =
		mRefreshRect.left =
		mRefreshRect.width =
		mRefreshRect.height = 0;
		return;
	}
	GWidget *p = parent;
	while (p) {
		if (!p->rect.Contains(refreshzone)) {
			//Debug(dprintf("ReallyDraw(%x): refreshzone out of parent widget rect\n", this));
			mRefreshRect.top =
			mRefreshRect.left =
			mRefreshRect.width =
			mRefreshRect.height = 0;
			return;
		}
		p = p->parent;
	}

	MaxRect(refreshzone, mRefreshRect, refreshzone);
	/*Debug(dprintf("ReallyDraw(%x): %d,%d,%d,%d -> %d,%d,%d,%d\n", this,
			R.left, R.top, R.width, R.height,
			refreshzone.left, refreshzone.top, refreshzone.width, refreshzone.height	);)*/

	EventDraw(refreshzone, TRUE);
	IRect inner_refreshzone(rect.left + mHborder,
							rect.top + mVborder,
							rect.width - mHborder * 2,
							rect.height - mVborder * 2);
	if (ClipRect(inner_refreshzone, inner_refreshzone, refreshzone)) {
		IRect child_refreshzone;
		ListIterator<GWidget> list(childs);
		while (!list.End()) {
			GWidget &child = list.Next();
			if ((mPushed && moveChildsOnPush) || (moveChildsNow)) {
				child.moveChildsNow = TRUE;
				child.mFreeze = TRUE;
				++child.rect.left;
				++child.rect.top;
				if (ClipRect(child_refreshzone, child.rect, inner_refreshzone)) {
					if (rect.Contains(child_refreshzone)) {
						child.ReallyDraw(child_refreshzone);
					}
				}
				child.mFreeze = FALSE;
				--child.rect.left;
				--child.rect.top;
			}
			else if (ClipRect(child_refreshzone, child.rect, inner_refreshzone)) {
				child.moveChildsNow = FALSE;
				if (rect.Contains(child_refreshzone)) {
					child.ReallyDraw(child_refreshzone);
				}
			}
		}
	}
	EventDraw(refreshzone, FALSE);
	mRefreshRect.top =
	mRefreshRect.left =
	mRefreshRect.width =
	mRefreshRect.height = 0;
}


//----------------------------------------------------------
void GWidget::EventDraw(const IRect&R, _BOOL BEFORECHILDS)
{
	if (Visible()) {
		display->SetClip(R);
		layout->Draw(*this, *resource, R, BEFORECHILDS);
		if (BEFORECHILDS) { Call(GEvent::OnDraw); }
	}
}


//------------------------------------------------------------
/*GPopupMenu* GWidget::PopupMenu()
{
	return popup;
}
*/


//------------------------------------------------------------
/*GPopupMenu* GWidget::SetPopupMenu(GPopupMenu *POPUP)
{
	popup = POPUP;
	if (popup)
	{
		popup->window = window;
		popup->EventCreate();
	}
	return popup;
}
*/


//------------------------------------------------------------
void GWidget::SetFont(NMO::IFont&F)
{
	font = &F;
	ResizeChilds();
	Refresh();
}


//------------------------------------------------------------
void GWidget::SetGroupIndex(_LONG IDX)
{
	mGroupIndex = IDX;
}

//------------------------------------------------------------
void GWidget::SetData(_PTR DATA)
{
	mUserData = DATA;
}


//------------------------------------------------------------
void GWidget::SetResource(GResource&R)
{
	resource = &R;
	Refresh();
}


//------------------------------------------------------------
_BOOL GWidget::DrawBackground() const {
	return drawBackground;
}

//------------------------------------------------------------
IRect& GWidget::ChildsRect() {
	return mChildsRect;
}


//------------------------------------------------------------
_BOOL& GWidget::Pointed() {
	return mPointed;
}


//------------------------------------------------------------
const GWidget::AlignmentType GWidget::Alignment() const {
	return alignment;
}


//------------------------------------------------------------
GWidget::WidgetType GWidget::Type() const {
	return type;
}


//------------------------------------------------------------
GWidget* GWidget::Parent() const {
	return parent;
}


//------------------------------------------------------------
_BOOL GWidget::Enabled() const {
	return mEnabled;
}


//------------------------------------------------------------
_BOOL GWidget::Focused() const {
	return focused;
}


//------------------------------------------------------------
const IRect& GWidget::Rect() const {
	return rect;
}


//------------------------------------------------------------
void GWidget::SetRect(_LONG L, _LONG T, _DWORD W, _DWORD H, _BOOL R)
{
	SetPos(L, T, R);
	SetSize(W, H, R);
}


//------------------------------------------------------------
void GWidget::SetRect(const IRect&R, _BOOL RE)
{
	SetRect(R.left, R.top, R.width, R.height, RE);
}


//------------------------------------------------------------
_BOOL& GWidget::Pushed() {
	return mPushed;
}


//------------------------------------------------------------
_BOOL& GWidget::Freeze() {
	return mFreeze;
}


//------------------------------------------------------------
_BOOL& GWidget::MoveChildsOnPush() {
	return moveChildsOnPush;
}


//------------------------------------------------------------
_BOOL& GWidget::Transparent() {
	return transparent;
}

//------------------------------------------------------------
_BOOL& GWidget::RedrawOnMouseEvent() {
	return redrawOnMouseEvent;
}


//------------------------------------------------------------
_DWORD GWidget::Padding() const {
	return mPadding;
}


//------------------------------------------------------------
_DWORD GWidget::VBorder() const {
	return mVborder;
}


//------------------------------------------------------------
_DWORD GWidget::HBorder() const {
	return mHborder;
}


//-----------------------------------------------------------
void GWidget::SetVBorder(_DWORD B) {
	mVborder = B;
	if (!mFreeze) { ResizeChilds(); }
	Refresh();
}


//-----------------------------------------------------------
void GWidget::SetHBorder(_DWORD B) {
	mHborder = B;
	if (!mFreeze) { ResizeChilds(); }
	Refresh();
}


//------------------------------------------------------------
_LONG GWidget::GroupIndex() const {
	return mGroupIndex;
}


//------------------------------------------------------------
_PTR GWidget::Data() const {
	return mUserData;
}


