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
#ifndef _GUI_SCROLLPANEL_H_
#define _GUI_SCROLLPANEL_H_

namespace NMO {

/*!
\brief		rectangular widget with horizontal and vertical scrollbars

\author		(c) 2001-2002 Henri Michelon
\version	$Id: GScrollBox.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GScrollBox: public GBox
{
public:
	GScrollBox();
	virtual ~GScrollBox() {};

	virtual GWidget& Add(GWidget&, AlignmentType = NONE, const Stringz& = "", _DWORD = 0);
	virtual GWidget* Add(GWidget*, AlignmentType = NONE, const Stringz& = "", _DWORD = 0);
	virtual void Drop(GWidget&);
	virtual void DropAll();
	virtual List<GWidget> &Childs();
	void SetResources(const Stringz&, const Stringz&, const Stringz&);

	/*! Return the widget which really contains the childs */
	GWidget &InnerBox();

protected:
	GWidget		mInnerBox;
	GVScrollBar	mVScroll;
	GHScrollBar	mHScroll;

	virtual void ComputeSums();
	virtual void EventResize(_BOOL);
	void OnVScrollChange(GWidget&, GEvent*);
	void OnHScrollChange(GWidget&, GEvent*);

	GScrollBox(WidgetType);
};


}

#endif
