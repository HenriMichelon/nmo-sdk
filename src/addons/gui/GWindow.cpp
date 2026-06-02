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

$Id: GWindow.cpp,v 1.2 2002/11/29 09:31:04 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
#include <nmo/WindowApplication.hpp>
#include "nmo/addons/gui/GWindow.hpp"
using namespace NMO;


//------------------------------------------------------------
GWindow::GWindow(): IWindow(),
	mWidget(NULL), mFocusedWidget(NULL)
{
	mFreeze = FALSE;
	mLayout = NULL;
	mDestroylayout = FALSE;
}


//------------------------------------------------------------
GWindow::~GWindow()
{
	Close();
	if (mWidget) { delete mWidget; }
	if (mLayout && mDestroylayout) { delete mLayout; }
}


//------------------------------------------------------------
void GWindow::UnFreeze(GWidget&W)
{
	ListIterator<GWidget> list(W.Childs());
	while (!list.End()) {
		UnFreeze(list.Next());
	}
	W.Freeze() = FALSE;
}

//------------------------------------------------------------
GWidget& GWindow::SetWidget(GWidget *WIDGET, const Stringz&RES, _DWORD PADDING)
{
	ASSERT(display);
	if (mLayout == NULL) { SetLayout(NULL); }
	if (WIDGET == NULL) {
		mWidget = new GPanel;
	}
	else {
		mWidget = WIDGET;
	}
	mWidget->mFreeze = TRUE;
	mWidget->display = display;
	mWidget->keyb = &keyb;
	mWidget->mPadding = PADDING;
	mWidget->window = this;
	mWidget->layout = mLayout;
	mWidget->font = &mWidget->layout->Font();
	mWidget->layout->AddResource(*mWidget, RES);
	mWidget->SetDrawBackground(TRUE);
	mWidget->EventCreate();
	mWidget->SetRect(0, 0);
	mWidget->SetSize(Width(), Height());
	mFocusedWidget = mWidget->SetFocus();
	UnFreeze(*mWidget);
	return *mWidget;
}


//------------------------------------------------------------
void GWindow::SetLayout(GLayout*LAYOUT)
{
	if (mLayout) { delete mLayout; }
	mLayout = LAYOUT;
	if (mLayout == NULL) { mLayout = GLayout::Create(); }
	ASSERT(mLayout);
	mDestroylayout = TRUE;

    List<Stringz> rgb;
	Stringz opt = mLayout->Option("color_background");
    if (opt.Len() > 0) {
        opt.Split(rgb, ',');
        if (rgb.Count() == 3) {
            ListIterator<Stringz> lstrgb(rgb);
            _BYTE red = (_BYTE)(_DWORD)lstrgb[1];
            _BYTE green = (_BYTE)(_DWORD)lstrgb[2];
            _BYTE blue = (_BYTE)(_DWORD)lstrgb[3];
			IRGBColor color(red, green, blue);
			SetBgColor(color);
        }
    }
}


//------------------------------------------------------------
void GWindow::SetLayout(GLayout&LAYOUT)
{
	SetLayout(&LAYOUT);
	mDestroylayout = FALSE;
}


//------------------------------------------------------------
void GWindow::EventCreate()
{
	OnCreate();
	if (mWidget == NULL) { SetWidget(); }
	if (mWidget != NULL ) { mWidget->ResizeChilds(); }
}


//------------------------------------------------------------
void GWindow::EventDestroy()
{
	if (mWidget) { mWidget->EventDestroy(); }
	if (handle == NULL) { return; }
	OnDestroy();
	mModalResult = -1;
}


//------------------------------------------------------------
_BOOL GWindow::EventQueryDestroy()
{
	return OnQueryDestroy();
}


//------------------------------------------------------------
void GWindow::EventDraw(const IRect&R)
{
	if (mFreeze) { return; }
	if (mWidget) { mWidget->Refresh(); }
	OnDraw(R);
	EndRefresh();
}


//------------------------------------------------------------
void GWindow::EventShow()
{
	if (mWidget) { mWidget->EventShow(); }
	OnShow();
	EndRefresh();
}


//------------------------------------------------------------
void GWindow::EventMove()
{
	if (mFreeze) { return; }
	mFreeze = TRUE;
	OnMove();
	mFreeze = FALSE;
}


//------------------------------------------------------------
void GWindow::EventResize()
{
	if (mFreeze) { return; }
	mFreeze = TRUE;
	if (mWidget) { mWidget->SetSize(Width(), Height()); }
	OnResize();
	if (!display->NativeDoubleBuffer()) {
		EndRefresh();
	}
	mFreeze = FALSE;
}


//------------------------------------------------------------
void GWindow::EventKeybDown(IKeyb::Key K)
{
/*	if ((K == keyb.KEY_TAB) && mWidget)
	{
//		mWidget->ClosePopup();
		if (mFocusedWidget)
			mFocusedWidget = mFocusedWidget->SetNextFocus();
		else
			mFocusedWidget = mWidget->SetFocus();
	}
	else*/ if (mFocusedWidget != NULL) {
		mFocusedWidget->EventKeybDown(K);
	}
	OnKeybDown(K);
	if (handle == NULL) { return; }
	if (!display->NativeDoubleBuffer()) {
		EndRefresh();
	}
}


//------------------------------------------------------------
void GWindow::EventKeybUp(IKeyb::Key K)
{
	if (mFocusedWidget != NULL) mFocusedWidget->EventKeybUp(K);
	OnKeybUp(K);
	if (handle == NULL) { return; }
	if (!display->NativeDoubleBuffer()) {
		EndRefresh();
	}
}


//------------------------------------------------------------
void GWindow::EventMouseDown(IMouse::Button B, _LONG X, _LONG Y)
{
	if (mWidget) {
		GWidget *newfocused = mWidget->EventMouseDown(B, X, Y);
		if (handle == NULL) { return; }
		if (newfocused && (newfocused != mFocusedWidget)) {
			if (mFocusedWidget != NULL) { mFocusedWidget->SetFocus(FALSE); }
			mFocusedWidget = newfocused;
		}
	}
	OnMouseDown(B, X, Y);
	if (handle == NULL) { return; }
	//if (!display->NativeDoubleBuffer()) {
		EndRefresh();
	//}
}


//------------------------------------------------------------
void GWindow::EventMouseUp(IMouse::Button B, _LONG X, _LONG Y)
{
	if (mWidget) {
		mWidget->EventMouseUp(B, X, Y);
		if (handle == NULL) { return; }
	}
	OnMouseUp(B, X, Y);
	if (handle == NULL) { return; }
	//if (!display->NativeDoubleBuffer()) {
		EndRefresh();
	//}
}


//------------------------------------------------------------
void GWindow::EventMouseMove(IMouse::Button B, _LONG X, _LONG Y)
{
	if ((mFocusedWidget != NULL) &&
		(mFocusedWidget->mouseMoveOnFocus)) {
		mFocusedWidget->EventMouseMove(B, X, Y);
	}
	else if (mWidget) {
		mWidget->EventMouseMove(B, X, Y);
	}
	if (handle == NULL) { return; }
	OnMouseMove(B, X, Y);
	if (handle == NULL) { return; }
	if (!display->NativeDoubleBuffer()) {
		EndRefresh();
	}
}


//------------------------------------------------------------
void GWindow::EventGotFocus()
{
	OnGotFocus();
}


//------------------------------------------------------------
void GWindow::EventLostFocus()
{
	OnLostFocus();
}


//------------------------------------------------------------
void GWindow::EndRefresh()
{
	if (mFreeze) { return; }
	mFreeze = TRUE;
	if (Visible() && mWidget) {
		StartRefresh();
		mWidget->FlushRefresh(mRefreshrect);
		if (mRefreshrect.width && mRefreshrect.height) {
			/*Debug(dprintf("EndRefresh: %d, %d, %d, %d\n",
					mRefreshrect.left, mRefreshrect.top,
					mRefreshrect.width, mRefreshrect.height);)*/
			RefreshDisplay(mRefreshrect);
		}
	}
	mFreeze = FALSE;
}


//------------------------------------------------------------
void GWindow::StartRefresh()
{
	mRefreshrect.left = mRefreshrect.top =
		mRefreshrect.width = mRefreshrect.height = 0;
}


//------------------------------------------------------------
void GWindow::Refresh()
{
	if (mWidget) { mWidget->Refresh(); }
	EndRefresh();
}


//------------------------------------------------------------
void GWindow::SetFocusedWidget(GWidget*W)
{
	mFocusedWidget = W;
}


//------------------------------------------------------------
_LONG GWindow :: ShowModal ()
{
	Show();
	WindowApplication &app = (WindowApplication&)Application::App();
	mModalResult = 0;
	while ((mModalResult == 0) && (!app.Terminated())) {
		app.ProcessNextEvent((IWindow*)this);
	}
	return 0;
}


//------------------------------------------------------------
GWidget& GWindow::Widget() {
	ASSERT(mWidget);
	return *mWidget;
};


//------------------------------------------------------------
GLayout* GWindow::Layout() const {
	return mLayout;
};


//------------------------------------------------------------
void GWindow::EventHide() {
	OnHide();
};



//------------------------------------------------------------
void GWindow :: SetModalResult(_LONG MR)
{
	Close();
	mModalResult = MR;
}
