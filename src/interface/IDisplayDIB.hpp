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

	Windows DIB implementation of the display device

	(c) 1998-2001 Henri Michelon 
	
$Id: IDisplayDIB.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------*/
#ifndef _WIN32_DISPDIB_H_
#define _WIN32_DISPDIB_H_

namespace NMO {

class IDisplayDIB : public IDisplayChunky
{
public:
	IDisplayDIB(DisplayType, HWND, HINSTANCE);
	virtual ~IDisplayDIB();

	virtual _BOOL CreateBuffers();
	virtual void DestroyBuffers();
	virtual void Refresh();
	virtual void SetClip(_LONG, _LONG, _DWORD, _DWORD);
	virtual void SetPenColor(const IRGBColor&);
	virtual void DrawPixel (_LONG, _LONG);
	virtual void DrawHLine (_LONG, _LONG, _DWORD);
	virtual void DrawLine (_LONG, _LONG, _LONG, _LONG);
	virtual void DrawRect(_LONG, _LONG, _DWORD, _DWORD);
	virtual void Clear();

	void Refresh(const IRect&);
private:
    HWND   		hwnd;
	HRGN		hrgn;
	HPEN		hpen;
	HBRUSH		hbrush;
	HPALETTE	m_hPalette;
	COLORREF	pencolor;
	SimpleDIB	dblBuffer;
	HINSTANCE	hInstance;

	BOOL MakePalette(IPalette&);
};

}

#endif // _WIN32_DISPDIB_H_
