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
#ifndef _GUI_ROUNDBUTTON_H_
#define _GUI_ROUNDBUTTON_H_

namespace NMO {

/*!
\brief		

\author		(c) 1998-2002 Henri Michelon
\version	$Id: GRoundButton.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GRoundButton: public GValueSelect
{
public:
	typedef enum
	{
		RB_TOP,
		RB_BOTTOM,
		RB_LEFT,
		RB_RIGHT
	} GRoundButtonType;

	GRoundButton(GRoundButtonType = RB_TOP, _DWORD = 0, _DWORD = 0, _DWORD = 1, _DWORD = 0);
	virtual ~GRoundButton();
	
	GRoundButtonType RoundButtonType() const;

private:
	GRoundButtonType 	type;
	_BOOL				onScroll;
	_LONG				scrollStart;
	
	virtual void EventMouseUp(IMouse::Button, _LONG, _LONG);
	virtual GWidget* EventMouseDown(IMouse::Button, _LONG, _LONG);
	virtual void EventMouseMove(IMouse::Button, _LONG, _LONG);
};

inline GRoundButton::GRoundButtonType GRoundButton::RoundButtonType() const { return type; };

}

#endif
