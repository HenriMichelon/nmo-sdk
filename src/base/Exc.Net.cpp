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

	(c) 2000-2001 Henri Michelon

$Id: Exc.Net.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------*/
#if defined(WIN32) || defined(_WIN32)
#using <System.WinForms.dll>
using namespace System::WinForms;
#include <nmo/NMO.hpp>

//--------------------------------------------------------
void NMOException::Display()
{
	Stringz t = sTitle;
	Console::Write(_msg);
	MessageBox::Show((_CHAR*)mMsg, (_CHAR*)t, 
					 MessageBox::IconExclamation | MessageBox::OK);
}
#endif
