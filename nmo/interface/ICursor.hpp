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
#ifndef _NMO_ICURSOR_H_
#define _NMO_ICURSOR_H_

namespace NMO {

/*!
\brief		Manipulation of the mouse cursor

\author		(c) 1998-2001 Henri Michelon 
\version	$Id: ICursor.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class ICursor: public NMOObject
{
public:
	typedef enum {
		CUR_DEFAULT,
		CUR_IBEAM,
		CUR_CROSSHAIR,
		CUR_HOURGLASS
	} CursorType;
	
	/*! Change the mouse cursor appearance.
		\param CursorType	: cursor type
		\param IWindow		: constraint appearance to a specific window,
								or to the application if NULL
	*/						
	static void SetCursor(CursorType = CUR_DEFAULT, IWindow * = NULL);
	
	/*! Return the cursor appearance for a window or for the application
		(if IWindow == NULL) */
	static CursorType GetCursor(IWindow * = NULL);

	static void Show(_BOOL = TRUE);
};

}

#endif

