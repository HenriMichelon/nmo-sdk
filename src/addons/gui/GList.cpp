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

$Id: GList.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;

//-------------------------------------
GList::GList(WidgetType T): 
	GWidget(T), prevSelected(0), selected(0)
{}




//-------------------------------------
_DWORD GList::Add(GWidget &S, GWidget::AlignmentType, const Stringz&)
{
	_DWORD index = widgets.Add(S);
	ComputeSums();
	GEventItem e(index, &S);
	Call(GEvent::OnInsertItem, &e);
	Refresh();
	return index;
}


//-------------------------------------
_DWORD GList::Add(GWidget *S, GWidget::AlignmentType, const Stringz&)
{
	_DWORD index = widgets.Add(S);
	ComputeSums();
	GEventItem e(index, S);
	Call(GEvent::OnInsertItem, &e);
	Refresh();
	return index;
}


//-------------------------------------
void GList::Drop(_DWORD I)
{
	if (I > 0) {
		GEventItem e(I, &Item(I));
		Call(GEvent::OnRemoveItem, &e);
		widgets.Drop(I);
		ComputeSums();
		Refresh();
	}
}


//-------------------------------------
void GList::DropAll()
{
	while (Count() > 0) {
		Drop(1);
	}
}


//-------------------------------------
GWidget& GList::Item(_DWORD I)
{
	ListIterator<GWidget> list(widgets);
	return list[I];
}



//-------------------------------------
_DWORD GList::Count()
{
	return widgets.Count();
}



//-------------------------------------
_DWORD GList::IndexSelected()
{
	return selected;
}



//-------------------------------------
GWidget* GList::Selected()
{
	if (selected) {
		return &Item(IndexSelected());
	}
	else {
		return NULL;
	}
}


//-------------------------------------
void GList::Select(_DWORD I)
{
	EventSelectItem(I);
}


//-------------------------------------
void GList::EventSelectItem(_DWORD I)
{
	if (I != selected) {
		prevSelected = I;
		selected = I;
		GEventItem e(I, NULL);
		if (I) { e.item = &Item(I); }
		Call(GEvent::OnSelectItem, &e);
		Refresh();
	}
}
