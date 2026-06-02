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

	(c) 2000-2002 Henri Michelon

$Id: IDisplayX11.hpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_X11_DISP_H_
#define _NMO_X11_DISP_H_

// Define this to force NMO to use XLib's drawing function
// instead of the DisplayChunky generic class
//#define USE_XLIB 1


namespace NMO {

class IDisplayX11 :
#ifdef USE_XLIB
	public IDisplay
#else
	public IDisplayChunky
#endif // USE_XLIB
{
public:
	IDisplayX11(DisplayType, IHandle);
	virtual ~IDisplayX11();

	void Refresh();
	void Refresh(const IRect&);

	_BOOL CreateBuffers();
	void DestroyBuffers();

#ifdef USE_XLIB
	void SetClip(_LONG, _LONG, _DWORD, _DWORD);
	void GetClip(_LONG&, _LONG&, _DWORD&, _DWORD&);
	void SetPenColor(const IRGBColor&);
	void SetTransColor(const IRGBColor&);
	void DrawPixel (_LONG, _LONG);
	void GetPixel (_LONG, _LONG, IRGBColor&);
	void DrawLine (_LONG, _LONG, _LONG, _LONG);
	void DrawHLine (_LONG, _LONG, _DWORD);
	void DrawRect(_LONG, _LONG, _DWORD, _DWORD);
	void DrawCircle (_LONG, _LONG, _DWORD, _BOOL = TRUE, _DWORD = 1);
	_BYTE * GetRect(_LONG&, _LONG&, _DWORD&, _DWORD&);
	void DrawNoTransNoClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransNoClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawNoTransClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void Clear();
	_DWORD Height() const;
	_DWORD Width() const;
#endif // USE_XLIB

private:
	_PTR mAbstractX11;
};

}

#endif // _NMO_X11_DISP_H_
