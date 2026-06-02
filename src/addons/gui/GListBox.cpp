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
						
$Id: GListBox.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;


//--------------------------------------------------------------------------
GListBox::GListBox(): GList(LISTBOX), 
	topselected(0), firstdisplayed(0)
{ 
	transparent = TRUE;
}



//---------------------------------------------
_LONG GListBox::maxDisplay(_DWORD S, _DWORD H)
{
	_DWORD	md = Count();
	_DWORD	y = 0;
	ListIterator<GWidget> list(widgets);
	for (_DWORD i=S; i<Count(); i++) {
		y += list[i+1].Height() + box.Padding()*2;
		if (y>=H) {
			md = i;
			break;
		}
	}
	return md;
}


//-------------------------------------
void GListBox::ComputeSums()
{
	innerHeight = box.Height() - box.VBorder()*2;
	totalHeight = 0;
	ListIterator<GWidget> list(widgets);
	while (!list.End()) {
		totalHeight += list.Next().Height() + box.Padding()*2;
	}
	vscroll.SetMax(totalHeight - innerHeight + vscroll.Step());
	//vscroll.Enable(firstdisplayed > 0); //(Count() - maxDisplay(firstdisplayed, innerHeight)) > 0);
}

//-------------------------------------
GWidget::AlignmentType GListBox::CorrectAlignment(GWidget::AlignmentType A)
{
	switch (A) {
	case RIGHT:
		return TOPRIGHT;
	case CENTER:
		return TOPCENTER;
	default:
		return TOPLEFT;
	}
}


//-------------------------------------
_DWORD GListBox::Add(GWidget &S, GWidget::AlignmentType A, const Stringz& RES)
{
	box.Add(S, CorrectAlignment(A), RES);
	return GList::Add(S, A, RES);
}


//-------------------------------------
_DWORD GListBox::Add(GWidget *S, GWidget::AlignmentType A, const Stringz& RES)
{
	box.Add(*S, CorrectAlignment(A), RES);
	return GList::Add(S, A, RES);
}

//-------------------------------------
void GListBox::Drop(_DWORD I)
{
	if (I > 0) {
		if ((I == IndexSelected()) && (I == widgets.Count())) {
			Select(I - 1);
		}
		ListIterator<GWidget> list(widgets);
		box.Drop(list[I]);
		GList::Drop(I);
	}
}


//--------------------------------------------------------------------------
void GListBox::SetResources(const Stringz&RBOX, const Stringz&RSCROLL, 
							const Stringz&RSEL)
{
	GWidget::Add(vscroll, RIGHT, RSCROLL);
	GWidget::Add(box, CLIENT, RBOX);
	box.Add(select, GWidget::NONE, RSEL);

	box.SetPadding(0);
	vscroll.SetStep(2);
	Connect(GEvent::OnKeybDown, this, GEventFunction(&GListBox::OnBoxKeybDown));
	box.Connect(GEvent::OnMouseDown, this, GEventFunction(&GListBox::OnBoxMouseDown));
	box.Connect(GEvent::OnResize, this, GEventFunction(&GListBox::OnBoxResize));
	vscroll.Connect(GEvent::OnValueChange, this, GEventFunction(&GListBox::OnVScrollChange));

	select.Show(FALSE);
	AllowFocus();
}


//--------------------------------------------------------------------------
void GListBox::EventSelectItem(_DWORD IDX)
{
	if (IDX == IndexSelected()) { IDX = 0; }
	Freeze() = TRUE;
	SetRectSelected(IDX);
	box.Refresh();
	Freeze() = FALSE;
	GList::EventSelectItem(IDX);
}


//--------------------------------------------------------------------------
void GListBox::SetRectSelected(_DWORD IDX)
{
	if (IDX > 0) {
		ListIterator<GWidget> list(widgets);
		topselected = box.Top() + box.VBorder();
		for (_DWORD idx=1; idx<IDX; idx++) {
			topselected += list[idx].Height() + box.Padding();
		}
		select.SetRect(box.Left() + box.HBorder(),
						topselected - vscroll.Value(),
						box.Width() - box.VBorder()*2,
						list[IDX].Height() + box.Padding()*2);
		select.Show(IDX > 0);
		list[IDX].Refresh();
	}
}


//--------------------------------------------------------------------------
void GListBox::OnBoxResize(GWidget&, GEvent*)
{
	ComputeSums();
	SetRectSelected(IndexSelected());
}


//--------------------------------------------------------------------------
void GListBox::OnBoxKeybDown(GWidget&, GEvent*e)
{
	if (Count()) {
		_DWORD idx = IndexSelected();
		_LONG max = maxDisplay(idx, box.Height());
		GEventKeyb *key = (GEventKeyb*)e;
		if ((key->key == keyb->KEY_UPARROW) && (idx>1))
			Select(idx-1);
		else if ((key->key == keyb->KEY_DOWNARROW) && (idx<Count()))
			Select(idx+1);
		else if ((key->key == keyb->KEY_PAGEUP) && (idx>1))	{
			if ((_LONG(idx) - vscroll.Value()) > 1)
				Select(vscroll.Value()+1);
			else if (vscroll.Value() < max)
				Select(1);
			else
				Select(vscroll.Value()-max+1);
		}
		else if ((key->key == keyb->KEY_PAGEDOWN) && (idx < Count())) {
			if ((_LONG(idx) - vscroll.Value()) < max)
				Select(vscroll.Value()+max);
			else if (idx > (Count() - max))
				Select(Count());
			else
				Select(idx+max);
		}
	}
}


//--------------------------------------------------------------------------
void GListBox::OnBoxMouseDown(GWidget&BOX, GEvent*e)
{
	if (Count()) {
		GEventMouse *mouse = (GEventMouse*)e;
		_DWORD y = (mouse->y - (BOX.Top() + BOX.VBorder())) +
					vscroll.Value();
		ListIterator<GWidget> list(widgets);
		while (!list.End()) {
			GWidget &w = list.Next();
			y -= BOX.Padding();
			if (y < w.Height()) {
				y = list.Index() - 1;
				break;
			}
			y -= w.Height();
		}
		BOX.Pushed() = FALSE; 
		if (y<=Count()) Select(y);
	}
}


//--------------------------------------------------------------------------
void GListBox::OnVScrollChange(GWidget&, GEvent*E)
{
	GEventValue *e = (GEventValue*)E;
	_LONG y = 0;
	ListIterator<GWidget> list(widgets);
	while (!list.End()) {
		GWidget &w = list.Next();
		y += w.Height() + box.Padding()*2;
		if (y > e->value) {
			firstdisplayed = list.Index()-1;
			break;
		}
	}
	box.ChildsRect().top = -e->value;
	box.ResizeChilds();	
	if (IndexSelected() != 0) {
		select.SetPos(select.Left(), topselected - e->value);
		Selected()->Refresh();
	}
	Refresh();
}
