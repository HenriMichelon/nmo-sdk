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

	(c) 2001 Henri Michelon 
						
$Id: GTabs.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/addons/GUI.hpp>


//--------------------------------------------------------------------------
GTabButton::GTabButton(): GCheckWidget(TABBUTTON)
{
	moveChildsOnPush = TRUE;
	redrawOnMouseEvent = TRUE;
	redrawOnMouseMove = TRUE;
	allowFocus = TRUE;
}


//--------------------------------------------------------------------------
GTabs::GTabs(): GList(TABS) { }


//-------------------------------------
void GTabs::SetResources(const Stringz&SUBRES)
{
	subresource = SUBRES;
}


//-------------------------------------
_DWORD GTabs::Add(GWidget &S, GWidget::AlignmentType A, const Stringz& RES)
{
	GTabButton *tab = new GTabButton;
	GWidget::Add(tab, GWidget::LEFT, subresource);
	tab->Add(S, A, RES);
	_DWORD idx = GList::Add(S, A, RES);
	if (IndexSelected() == 0) {
		Select(1);
	}
	return idx;
}


//-------------------------------------
_DWORD GTabs::Add(GWidget *S, GWidget::AlignmentType A, const Stringz& RES)
{
	GTabButton *tab = new GTabButton;
	GWidget::Add(tab, GWidget::LEFT, subresource);
	tab->Add(*S, A, RES);
	_DWORD idx = GList::Add(S, A, RES);
	if (IndexSelected() == 0) {
		Select(1);
	}
	return idx;
}


//-------------------------------------
void GTabs::EventResize(_BOOL R)
{
	GList::EventResize(R);
	if (childs.Count() > 0) {
		tabwidth = MAX(_LONG(Width() / (childs.Count() ) - mPadding * 2), 
					  0l);
		ListIterator<GWidget> lst(childs);
		while (!lst.End()) {
			lst.Next().SetSize(tabwidth, 0);
		}
	}
}


//-------------------------------------
void GTabs::Drop(_DWORD I)
{
	if (I == 0) { return; }
	GList::Drop(I);
	Drop(I);
}


//--------------------------------------------------------------------------
void GTabs::EventSelectItem(_DWORD IDX)
{
	if (IDX > 0) {
		ListIterator<GWidget> lst(childs);
		while (!lst.End()) {
			GTabButton &w = (GTabButton&) lst.Next();
			w.SetState(GCheckWidget::UNCHECK);
		}
		((GTabButton&)lst[IDX]).SetState(GCheckWidget::CHECK);
		Refresh(); 
	}
	GList::EventSelectItem(IDX);
}


//--------------------------------------------------------------------------
void GTabs::ComputeSums()
{
	if (childs.Count() > 0) {
		tabwidth = MAX(_LONG(Width() / (childs.Count() ) - mPadding * 2),
					  0l);
	}
	else {
		tabwidth = 0;
	}
}



//--------------------------------------------------------------------------
GWidget* GTabs::EventMouseDown(IMouse::Button BTN, _LONG X, _LONG Y)
{
	GWidget *w = GList::EventMouseDown(BTN, X, Y);
	if (rect.Contains(X, Y)) {
		_DWORD x = X - Left() - mPadding;
		if (x < (tabwidth * childs.Count())) {
			Select(x / tabwidth + 1);
		}
	}
	return w;
}
