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
				
$Id: GText.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;


//----------------------------------------------
GText::GText(UStringz C): GPanel(TEXT), text(C), autoSize(TRUE)
{
	allowChilds = FALSE;
	drawBackground = FALSE;
}


//----------------------------------------------
void GText::SetText(const UStringz &C)
{
	PRE(font, "GText::SetText: Widget must be added to another widget before setting text");
	text = C;
	if (parent) { parent->Refresh(rect); }
	ComputeSize();
	if (!parent) { Refresh(); }
	Call(GEvent::OnTextChange);
}


//----------------------------------------------
void GText::SetAutoSize(_BOOL A)
{
	if (autoSize == A) return;
	autoSize = A;
	ComputeSize();
}


//---------------------------------------------
void GText::EventCreate()
{
	ComputeSize();
	GPanel::EventCreate();
}


//----------------------------------------------
void GText::ComputeSize()
{	
	ASSERT(font);
	if (text.Len()) {
		if (autoSize) {
			SetSize(font->Width(text), font->Height());
		}
		else {
			Freeze() = TRUE;
			rect.height = font->Height();
			ResizeChilds();
			Freeze() = FALSE;
		}
	}
}


