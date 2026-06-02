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

$Id: GCheckWidget.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
using namespace NMO;


//----------------------------------------------
GCheckWidget::GCheckWidget(GWidget::WidgetType T): 
			GWidget(T), state(UNCHECK)
{
}


//----------------------------------------------
GWidget* GCheckWidget::EventMouseDown(IMouse::Button B, _LONG X, _LONG Y)
{
	if (rect.Contains(X, Y)) {
		if (state == CHECK)
			SetState(UNCHECK);
		else
			SetState(CHECK);
	}
	return GWidget::EventMouseDown(B, X, Y);
}


//----------------------------------------------
void GCheckWidget::SetState(GCheckWidget::CheckState S)
{
	if (state == S) return;
	state = S;
	ResizeChilds();
	Refresh();
	GEventState stat(S);
	Call(GEvent::OnStateChange, &stat);
}


//----------------------------------------------
GCheckButton::GCheckButton(): GCheckWidget(GWidget::CHECKBUTTON)
{
}


//----------------------------------------------
void GCheckButton::EventCreate()
{
	Add(box, CLIENT, resource->Resource());
	box.Add(checkmark, CLIENT, resource->Resource());
	allowChilds = FALSE;
	checkmark.Show(State() == CHECK);
	GCheckWidget::EventCreate();
}


//----------------------------------------------
void GCheckButton::SetState(CheckState S)
{
	checkmark.Show(State() == UNCHECK);
	GCheckWidget::SetState(S);
}
