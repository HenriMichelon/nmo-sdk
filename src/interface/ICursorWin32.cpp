/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://www.nmo-sdk.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1998-2001 Henri Michelon 

$Id: ICursorWin32.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <nmo/Interface.hpp>
# include <windows.h>
#endif


//------------------------------------------------------
void ICursor::SetCursor(CursorType CURSOR, IWindow *)
{
	LPSTR  wintype;
	switch (CURSOR) {
	case CUR_IBEAM: wintype = IDC_IBEAM; break;
	case CUR_CROSSHAIR: wintype = IDC_CROSS; break;
	case CUR_HOURGLASS: wintype = IDC_WAIT; break;
	default: wintype = IDC_ARROW; break;
	}
	HCURSOR cursor = ::LoadCursor(NULL, wintype);
	if (cursor) { ::SetCursor(cursor); }
}


//------------------------------------------------------
ICursor::CursorType ICursor::GetCursor(IWindow *)
{
	return CUR_DEFAULT;
}


//------------------------------------------------------
void ICursor::Show(_BOOL SHOW)
{
	::ShowCursor(SHOW);
}


#endif
