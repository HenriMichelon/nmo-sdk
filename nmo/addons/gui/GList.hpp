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
#ifndef _GUI_LIST_H_
#define _GUI_LIST_H_

namespace NMO {

/*!
\brief		Super class for all list-of-widgets based widgets

\author		(c) 1998-2002 Henri Michelon 
\version	$Id: GList.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GList: public GWidget
{
public:
	GList(WidgetType);
	virtual ~GList() {};
	
	virtual _DWORD Add(GWidget&, AlignmentType = LEFT, const Stringz& = "");
	virtual _DWORD Add(GWidget*, AlignmentType = LEFT, const Stringz& = "");

	// Remove a string to the list
	//	_LONG : index of string to remove (1 based)
	virtual void Drop(_DWORD);

	virtual void DropAll();

	// Get an string
	//	_LONG : index of string to get (1 based)
	GWidget& Item(_DWORD);

	// Get number of the strings in the list
	_DWORD Count();

	// get the currently selected string index (1 based)
	//	return 0 if no selection
	_DWORD IndexSelected();

	// get the currently selected string
	// return NULL result if no selection
	GWidget* Selected();

	// select an item in the list
	//	_LONG : item index (1 based), 0 = no selection
	void Select(_DWORD);

	List<GWidget>& Items();

protected:
	_DWORD			prevSelected;
	List<GWidget> 	widgets;

	virtual void ComputeSums() = 0;
	virtual void EventSelectItem(_DWORD);

private:
	_DWORD			selected;
};

inline List<GWidget>& GList::Items() { return widgets; }

}

#endif
