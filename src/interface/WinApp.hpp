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

	(c) 1998-2000 Henri Michelon 

$Id: WinApp.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-----------------------------------------------------------------*/
#ifndef _NMO_WIN32_WINAPP_H_
#define _NMO_WIN32_WINAPP_H_

namespace NMO {

class WinApp
{
public:
	Stringz			className;
	WNDCLASS		wndclass;
	HFONT			font;
	HINSTANCE		hInst;
	LPDIRECTDRAW	lpDD;
	_BOOL			quit;

	static LRESULT CALLBACK WindowProc(HWND, UINT,WPARAM, LPARAM);
};

#define WINAPP(x) ((WinApp*)(((WindowApplication&)(Application::App())).mWindowApplicationAbstract))->x

}

#endif
