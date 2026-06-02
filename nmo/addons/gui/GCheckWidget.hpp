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
#ifndef _GUI_CHECKWIDGET_H_
#define _GUI_CHECKWIDGET_H_

namespace NMO {

/*!
\brief		Super class for all checkable widgets

 
\author		(c) 1998-2002 Henri Michelon 
\version	$Id: GCheckWidget.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GCheckWidget: public GWidget
{
public:
	//! State of the widget. Change on user action.
	typedef enum
	{
		CHECK,		//! Checked (aka ON)
		UNCHECK		//! Unchecked (aka OFF)
	} CheckState;

	//! Return current state of the widget
	CheckState State() const;

	//! Change the state of the widget
	virtual void SetState(CheckState);

protected:
	GCheckWidget(WidgetType);

	virtual GWidget* EventMouseDown(IMouse::Button, _LONG, _LONG);

private:
	CheckState	state;
};

inline GCheckWidget::CheckState GCheckWidget::State() const { return state; }

}

#endif
