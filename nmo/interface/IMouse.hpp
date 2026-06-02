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
#ifndef _IMOUSE_H_
#define _IMOUSE_H_

namespace NMO {

/*!
\brief		Manage mouse position and constants


\author		(c) 1999-2001 Henri Michelon
\version	$Id: IMouse.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//----------------------------------------------------------------------

	class IMouse: public NMOObject
{
public:
	/*! Button constants */
	typedef enum
	{
		/*! no button */
		NONE,
		/*! left button */
		LEFT,
		/*! middle button, if supported, or combination of key+button */
		MIDDLE,
		/*! right button, if supported, or combination of key+button */
		RIGHT
	} Button;

	/*! X Position of the mouse cursor.
		relative to the upper left corner of the screen.
		Changing this value have no effects on the mouse.
	*/
	static _DWORD& XPos();

	/*! X Position of the mouse cursor.
		relative to the upper left corner of the screen.
		Changing this value have no effects on the mouse.
	*/
	static _DWORD& YPos();

	/*static _BOOL isDown(Button=LEFT);
	static _BOOL isPushed(Button=LEFT);
	static void SetButton(Button=LEFT, _BOOL=TRUE);*/

private:
	static _DWORD	mouseX, mouseY;
	/*static _BOOL	rButton, lButton;
	static _BOOL	rpButton, lpButton;*/
};

inline _DWORD& IMouse::XPos()
{ return mouseX; };

inline _DWORD& IMouse::YPos()
{ return mouseY; };
/*
inline _BOOL IMouse::isDown(Button B)
{
	if (B == LEFT)
		return lButton;
	else
		return rButton;
}
*/
}

#endif
