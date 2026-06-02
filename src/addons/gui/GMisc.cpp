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
						
$Id: GMisc.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;
#include <nmo/addons/GUI.hpp>

//---------------------------------------------------------------------
GLine::GLine(GLine::LineStyle K): 
	GWidget(LINE), style(K)
{
	allowChilds = FALSE;
}


//---------------------------------------------------------------------
void GLine::SetStyle(GLine::LineStyle K)
{
	if (style != K)	{
		style = K;
		ResizeChilds();
		Refresh();
	}
}



//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
GFrame::GFrame(const UStringz& STR): GPanel(GWidget::FRAME)
{
	text = STR;
}


//----------------------------------------------
void GFrame::SetText(const UStringz &C)
{
	PRE(font, "GFrame::SetText: Widget must be added to another widget before setting text");
	text = C;
	ResizeChilds();
	Refresh();
	Call(GEvent::OnTextChange);
}


//----------------------------------------------
GArrow::GArrow(ArrowKind K): GWidget(ARROW), kind(K)
{
	allowChilds = FALSE;
}


//----------------------------------------------
void GArrow::SetKind(GArrow::ArrowKind KIND)
{
	if (kind != KIND) {
		kind = KIND;
		ResizeChilds();
		Refresh();
	}
}


//----------------------------------------------
GCheckmark::GCheckmark(): GWidget(CHECKMARK), kind(CROSS)
{
	allowChilds = FALSE;
}


//----------------------------------------------
void GCheckmark::SetKind(GCheckmark::CheckmarkKind KIND)
{
	kind = KIND;
	ResizeChilds();
	Refresh();
}




