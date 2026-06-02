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

		GDI implementation of the display device

		(c) 1998 Henri Michelon 
	
$Id: IDisplayGDI.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-------------------------------------------------------------------------*/
#ifndef _DIB_DISPGDI_H_
#define _DIB_DISPGDI_H_


namespace NMO {

class IDisplayGDI : public IDisplayChunky
{
private:
    HWND   		hwnd;
	HINSTANCE	hInstance;
	BITMAPINFOHEADER BIH;

	HPALETTE	m_hPalette;
	BOOL		MakePalette();
	HPEN		pen;
	HBRUSH		brush;
	HRGN		hrgn;

public:
	IDisplayGDI(HWND, HINSTANCE);
	virtual ~IDisplayGDI();
	void DestroyBuffer();

	_BOOL Initialize();
	void SetClip(_DWORD, _DWORD, _DWORD, _DWORD);
	void DrawNoTransNoClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransNoClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawNoTransClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawLine (_LONG , _LONG , _LONG , _LONG );
	void DrawHLine (_LONG, _LONG, _DWORD);
	void DrawRect(_LONG, _LONG, _LONG, _LONG);

	void SetPenColor(_DWORD);
	void ClearPage();
	inline void FlipBuffer() {};
};

}

#endif // _DIB_DISPGDI_H_
