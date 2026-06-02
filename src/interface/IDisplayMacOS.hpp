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

	(c) 2000-2001 Henri Michelon

$Id: IDisplayMacOS.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_INTERFACE_DISPMACOS_H_
#define _NMO_INTERFACE_DISPMACOS_H_


namespace NMO {

class IDisplayMacOS : public IDisplayChunky
{
public:
	IDisplayMacOS(DisplayType, WindowRef);
	virtual ~IDisplayMacOS();

	/*virtual void SetClip(_LONG, _LONG, _DWORD, _DWORD);
	virtual void GetClip(_LONG&, _LONG&, _DWORD&, _DWORD&);
	virtual void SetPenColor(const IRGBColor&);
	virtual void SetTransColor(const IRGBColor&);
	virtual void TransColor(IRGBColor&);
	virtual void PenColor(IRGBColor&);
	virtual _DWORD Height() const;
	virtual _DWORD Width() const;*/
	virtual void GetPixel (_LONG, _LONG, IRGBColor&);
	virtual void DrawPixel (_LONG, _LONG);
	virtual void DrawLine (_LONG, _LONG, _LONG, _LONG);
	virtual void DrawHLine (_LONG, _LONG, _DWORD);
	virtual void DrawRect(_LONG, _LONG, _DWORD, _DWORD);
	virtual void Clear();
	virtual _BYTE * GetRect(_LONG&, _LONG&, _DWORD&, _DWORD&);
	virtual void DrawNoTransNoClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	virtual void DrawTransNoClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	virtual void DrawNoTransClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	virtual void DrawTransClip(const void *, _LONG, _LONG, _DWORD, _DWORD);
	virtual _BOOL NativeDoubleBuffer() const;
	
	virtual void Refresh();
	virtual void Refresh(const IRect&);
	virtual _BOOL CreateBuffers();
	virtual void DestroyBuffers();		
	
	// IDisplayMacOS specific
	void QueryPixmap();
	
	static _BOOL &Lock();
	
private:
	static _BOOL locked;
	Rect		cliprect;
	CGrafPtr	drawport;
	CGrafPtr	windowport;
	GWorldPtr	gworld;
	WindowRef	window;
};

inline _BOOL& IDisplayMacOS::Lock() { return locked; }
inline void IDisplayMacOS::QueryPixmap()
{
	VALIDATETHIS;
#if TARGET_API_MAC_CARBON == 1
	LockPixels(GetPortPixMap(drawport));
	DrawSurface = (_BYTE*)GetPixBaseAddr(GetPortPixMap(drawport));		
#else
	LockPixels(drawport->portPixMap);
	DrawSurface = (_BYTE*)GetPixBaseAddr(drawport->portPixMap);		
#endif
}


}

#endif // _NMO_INTERFACE_DISPMACOS_H_
