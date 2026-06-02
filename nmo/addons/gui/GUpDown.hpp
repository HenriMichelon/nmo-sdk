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
#ifndef _GUI_UPDOWN_H_
#define _GUI_UPDOWN_H_

namespace NMO {

/*!
\brief		Up & down buttons
			
			Does not support VCENTER, LEFT & RIGHT alignments
			(auto converted to CENTER, LEFTCENTER & RIGHTCENTER)

\author		(c) 1998-2002 Henri Michelon
\version	$Id: GUpDown.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GUpDown: public GValueSelect
{
public:
	GUpDown(_DWORD = 0, _DWORD = 0, _DWORD = 0, _DWORD = 1);
	virtual ~GUpDown();

	void SetResources(const Stringz&, const Stringz&);

private:
	Alarm	*btnUpAlarm;
	Alarm	*btnDownAlarm;
	GButton	btnUp;
	GButton	btnDown;
	
	virtual void EventResize(_BOOL);
	virtual void EventMouseUp(IMouse::Button, _LONG, _LONG);
	virtual void EventMouseMove(IMouse::Button, _LONG, _LONG);

	void OnBtnUpDown(GWidget&, GEvent*);
	void OnBtnUpTimer(_DWORD);
	void OnBtnDownDown(GWidget&, GEvent*);
	void OnBtnDownTimer(_DWORD);
	void EventRangeChange();
};

}

#endif
