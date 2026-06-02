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
	
$Id: IDisplayDirectDraw.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------*/
#ifndef _DISPLAY_DIRECTX_H_
#define _DISPLAY_DIRECTX_H_

namespace NMO {


class IDisplayDirectDraw : public IDisplayChunky
{
public:
	IDisplayDirectDraw(DisplayType, HWND, HINSTANCE, LPDIRECTDRAW);
	virtual ~IDisplayDirectDraw();

	virtual _BOOL CreateBuffers();
	virtual void DestroyBuffers();
	virtual void Refresh();
	virtual void Refresh(const IRect&);

private:
    HWND   				hwnd;			// main windows handle
	HINSTANCE			hInstance;
	LPDIRECTDRAW		lpDD;           // DirectDraw object

	LPDIRECTDRAWCLIPPER lpddClipper;
	LPDIRECTDRAWSURFACE	lpDDSPrimary;   // DirectDraw primary surface
	LPDIRECTDRAWSURFACE	lpDDSBack;      // DirectDraw back surface
	//_BYTE				*primarySurface;
	//_BYTE				*backSurface;
	//_BOOL				flipFlag;

};

}

#endif
