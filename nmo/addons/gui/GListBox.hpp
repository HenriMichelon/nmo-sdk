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
#ifndef _NMO_GUI_LISTBOX_H_
#define _NMO_GUI_LISTBOX_H_

namespace NMO {

/*!
\brief		Text list with scrollbar

\author		(c) 1998-2002 Henri Michelon
\version	$Id: GListBox.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GListBox: public GList
{
public:
	GListBox();
	virtual ~GListBox() {};

	virtual _DWORD Add(GWidget&, AlignmentType = LEFT, const Stringz& = "");
	virtual _DWORD Add(GWidget*, AlignmentType = LEFT, const Stringz& = "");
	virtual void Drop(_DWORD);

	void SetResources(const Stringz&, const Stringz&, const Stringz&);

	GSelection& SelectionWidget() const;

private:
	GSelection	select;
	_DWORD		topselected;
	_DWORD		innerHeight;
	_DWORD		totalHeight;
	_DWORD		firstdisplayed;
	GBox		box;
	GVScrollBar	vscroll;

	AlignmentType CorrectAlignment(AlignmentType);

	_LONG maxDisplay(_DWORD, _DWORD);
	void ComputeSums();
	void EventSelectItem(_DWORD);
	void SetRectSelected(_DWORD);

	void OnBoxResize(GWidget&, GEvent*);
	void OnBoxMouseDown(GWidget&, GEvent*);
	void OnBoxKeybDown(GWidget&, GEvent*);
	void OnVScrollChange(GWidget&, GEvent*);
};

inline GSelection& GListBox::SelectionWidget() const { return (GSelection&)select; }

}

#endif
