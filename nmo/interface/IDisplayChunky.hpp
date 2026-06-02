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
#ifndef _INTERFACE_DISP_CHUNKY_H_
#define _INTERFACE_DISP_CHUNKY_H_

namespace NMO {

/*!
\brief		Generic chunky display.

			2D Display device : generic CHUNKY display.
			Mainly used for developement purpose.
			Never used directly.

\author		(c) 1998-2001 Henri Michelon 
\version	$Id: IDisplayChunky.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------


class IDisplayChunky : public IDisplay
{
public:
	IDisplayChunky(DisplayType);
	virtual ~IDisplayChunky() {};

	virtual void SetClip(_LONG, _LONG, _DWORD, _DWORD);
	virtual void GetClip(_LONG&, _LONG&, _DWORD&, _DWORD&);
	virtual void SetPenColor(const IRGBColor&);
	virtual void SetTransColor(const IRGBColor&);
	virtual void DrawPixel (_LONG, _LONG);
	virtual void GetPixel (_LONG, _LONG, IRGBColor&);
	virtual void DrawLine (_LONG, _LONG, _LONG, _LONG);
	virtual void DrawHLine (_LONG, _LONG, _DWORD);
	virtual void DrawRect(_LONG, _LONG, _DWORD, _DWORD);
	virtual void DrawCircle (_LONG, _LONG, _DWORD, _BOOL = TRUE, _DWORD = 1);
	virtual _BYTE * GetRect(_LONG&, _LONG&, _DWORD&, _DWORD&);
	virtual void DrawNoTransNoClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	virtual void DrawTransNoClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	virtual void DrawNoTransClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	virtual void DrawTransClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	virtual void Clear();
	virtual _DWORD Height() const;
	virtual _DWORD Width() const;

protected:
	_BYTE *DrawSurface;

	// clipping region
	_LONG XClipStart;
	_LONG YClipStart;
	_LONG XClipEnd;
	_LONG YClipEnd;

	// size of the display
	_DWORD DispWidth;
	_DWORD DispHeight;

	// Pitch of the display: distance to next line
	_DWORD DispPitch;

	_DWORD penColor;
	_DWORD transColor;

	void DrawNoTransNoClip_8(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawNoTransNoClip_16(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawNoTransNoClip_24(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawNoTransNoClip_32(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransNoClip_8(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransNoClip_16(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransNoClip_24(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransNoClip_32(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawNoTransClip_8(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawNoTransClip_16(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawNoTransClip_24(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawNoTransClip_32(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransClip_8(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransClip_16(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransClip_24(const void *, _LONG, _LONG, _DWORD, _DWORD);
	void DrawTransClip_32(const void *, _LONG, _LONG, _DWORD, _DWORD);
};

inline _DWORD IDisplayChunky :: Height() const { return DispHeight; }
inline _DWORD IDisplayChunky :: Width() const { return DispWidth; }

}

#endif
