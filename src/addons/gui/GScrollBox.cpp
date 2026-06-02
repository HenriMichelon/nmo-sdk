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

$Id: GScrollBox.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;

//--------------------------------------------------------------------------
GBox :: GBox(): GPanel (BOX)
{
}

//--------------------------------------------------------------------------
GBox :: GBox(WidgetType T): GPanel(T)
{
}


//--------------------------------------------------------------------------
GScrollBox :: GScrollBox(): GBox(SCROLLBOX)
{
	transparent = TRUE;
	//drawBackground = FALSE;
	mHborder = 1;
	mVborder = 1;
}


//---------------------------------------------
GScrollBox :: GScrollBox(WidgetType T): GBox(T)
{
	transparent = TRUE;
	//drawBackground = FALSE;
	mHborder = 1;
	mVborder = 1;
}


//-------------------------------------
GWidget& GScrollBox :: Add(GWidget&S, AlignmentType A, const Stringz&RES, _DWORD PAD)
{
	GWidget & w = mInnerBox.Add(S, A, RES, PAD);
	ComputeSums();
	return w;
}


//-------------------------------------
GWidget* GScrollBox :: Add(GWidget*S, AlignmentType A, const Stringz&RES, _DWORD PAD)
{
	GWidget * w = mInnerBox.Add(S, A, RES, PAD);
	ComputeSums();
	return w;
}

//-------------------------------------
void GScrollBox:: Drop(GWidget&WIDGET)
{
	mInnerBox.Drop(WIDGET);
	ComputeSums();
}


//-------------------------------------
void GScrollBox:: DropAll()
{
	mInnerBox.DropAll();
	ComputeSums();
}


//--------------------------------------------------------------------------
void GScrollBox::SetResources(const Stringz&RBOX, const Stringz&RVSCROLL,
							const Stringz&RHSCROLL)
{
	GWidget::Add(mVScroll, RIGHTTOP, RVSCROLL);
	GWidget::Add(mHScroll, BOTTOMLEFT, RHSCROLL);
	GWidget::Add(mInnerBox, CLIENT, RBOX, Padding());
	SetPadding(0);

	mInnerBox.Transparent() = TRUE;
	mInnerBox.SetDrawBackground(FALSE);

	mVScroll.SetStep(4);
	mVScroll.Connect(GEvent::OnValueChange,
					this,
					GEventFunction(&GScrollBox::OnVScrollChange));

	mHScroll.SetStep(4);
	mHScroll.Connect(GEvent::OnValueChange,
					this,
					GEventFunction(&GScrollBox::OnHScrollChange));

	AllowFocus();
}


//-------------------------------------
void GScrollBox :: ComputeSums()
{
	if (mInnerBox.Width() && mInnerBox.Height()) {
		_DWORD maxwidth = 0;
		_DWORD maxheight = 0;
		_LONG leftoffset = mInnerBox.Padding() - mInnerBox.Left() - mInnerBox.HBorder();
		_LONG topoffset = mInnerBox.Padding() - mInnerBox.Top()  - mInnerBox.VBorder();
		ListIterator<GWidget> list(mInnerBox.Childs());
		while (!list.End()) {
			GWidget &wdgt = list.Next();
			_DWORD right = wdgt.Left() + leftoffset + wdgt.Width();
			_DWORD bottom = wdgt.Top() + topoffset + wdgt.Height();
			maxwidth = MAX(maxwidth, right);
			maxheight = MAX(maxheight, bottom);
		}
		mHScroll.SetMax(maxwidth - mInnerBox.Width());
		mVScroll.SetMax(maxheight - mInnerBox.Height());
	}
	else {
		mHScroll.SetMax(0);
		mVScroll.SetMax(0);
	}
}


//--------------------------------------------------------------------------
void GScrollBox :: EventResize(_BOOL R)
{
	mVScroll.SetSize(mVScroll.Width(), Height() - mHborder * 2 - mVScroll.Width());
	mHScroll.SetSize(Width() - mVborder * 2 - mHScroll.Height(), mHScroll.Height());
	_DWORD oldv = mVScroll.Value();
	_DWORD oldh = mHScroll.Value();
	mVScroll.SetValue(0);
	mHScroll.SetValue(0);
	GBox::EventResize(R);	
	ComputeSums();
	mVScroll.SetValue(oldv);
	mHScroll.SetValue(oldh);
}


//--------------------------------------------------------------------------
void GScrollBox :: OnVScrollChange(GWidget&, GEvent*E)
{
	GEventValue *e = (GEventValue*)E;
	mInnerBox.ChildsRect().top = -e->value;
	mInnerBox.ResizeChilds();
	Refresh();
}


//--------------------------------------------------------------------------
void GScrollBox :: OnHScrollChange(GWidget&, GEvent*E)
{
	GEventValue *e = (GEventValue*)E;
	mInnerBox.ChildsRect().left = -e->value;
	mInnerBox.ResizeChilds();
	Refresh();
}

//--------------------------------------------------------------------------
NMO::List<GWidget> & GScrollBox :: Childs()
{
	return mInnerBox.Childs();
}


//--------------------------------------------------------------------------
GWidget & GScrollBox :: InnerBox()
{
	return mInnerBox;
}
