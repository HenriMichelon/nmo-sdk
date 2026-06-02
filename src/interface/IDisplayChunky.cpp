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

	(c) 1998-2002 Henri Michelon 

$Id: IDisplayChunky.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Interface.hpp>
using namespace NMO;

// -------------------------------
IDisplayChunky::IDisplayChunky(DisplayType T): 
	IDisplay(T),DrawSurface(NULL),
				XClipStart(0),
				YClipStart(0),
				XClipEnd(0),
				YClipEnd(0),
				DispWidth(0),
				DispHeight(0),
				DispPitch(0),
				penColor(0),
				transColor(0)
{
}


//--------------------------------------------------------------
void IDisplayChunky::GetClip(_LONG&X1, _LONG&Y1, _DWORD&X2, _DWORD&Y2)
{
	VALIDATETHIS;
	X1 = XClipStart;
	Y1 = YClipStart;
	X2 = XClipEnd - XClipStart +1;
	Y2 = YClipEnd - YClipStart +1;
}


//--------------------------------------------------------------
void IDisplayChunky :: SetClip(_LONG Xb, _LONG Yb, _DWORD W, _DWORD H)
{
	VALIDATETHIS;
	XClipStart = MAX(Xb, 0);
	YClipStart = MAX(Yb, 0);
	XClipEnd = MIN(XClipStart + W -1, DispWidth - 1);
	YClipEnd = MIN(YClipStart + H -1, DispHeight - 1);
}


// -------------------------------
void IDisplayChunky :: SetTransColor(const IRGBColor&C) 
{ 
	transColor = IColorSpace::colorSpace.Color(C); 
	IDisplay::SetTransColor(C);
}


// -------------------------------
void IDisplayChunky :: SetPenColor(const IRGBColor&C) 
{  
	penColor = IColorSpace::colorSpace.Color(C); 
	IDisplay::SetPenColor(C);
}



// -------------------------------
_BYTE *IDisplayChunky :: GetRect(_LONG&X, _LONG&Y, _DWORD&W, _DWORD&H)
{
	_LONG YCut;
	_LONG XCut;
	_LONG YEnd;
	_LONG XEnd;

	if ((Y > YClipEnd) || ((Y + ((_LONG)H)) <= YClipStart) || ((X + ((_LONG)W)) <= XClipStart) || (X > XClipEnd))
      return NULL;

	YCut = YClipStart - Y;
	XCut = XClipStart - X;
	YEnd = YClipEnd - Y + 1;
	XEnd = XClipEnd - X + 1;

	if (YCut > 0)
		H -= YCut;
	if (YEnd < (_LONG)H)
		H = YEnd;
	if (XCut > 0)
		W -= XCut;
	if (XEnd < (_LONG)W)
		W = XEnd;

	X = MAX(X, XClipStart);
	Y = MAX(Y, YClipStart);
	register _BYTE *Src = DrawSurface + 
				(X * IColorSpace::colorSpace.BytesPerPixel()) + 
				(Y * DispPitch);
	_BYTE *Dest = new _BYTE[W*H*IColorSpace::colorSpace.BytesPerPixel()];
	register _BYTE *pDest = Dest;
	_DWORD w = W * IColorSpace::colorSpace.BytesPerPixel();
	_DWORD h = H;
	do
	{
		Memory::CopyDWORD(pDest, Src, w);
		pDest += w;
		Src += DispPitch;
	}
	while (--h);
	return Dest;
}



// -------------------------------
void IDisplayChunky :: DrawNoTransNoClip_8(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	register _BYTE *TmpPic = (_BYTE *)Src;
	register _BYTE *Dest = DrawSurface + X + ( Y * DispPitch );
	do
	{
		Memory::Copy(Dest, TmpPic, W);
		TmpPic += W;
		Dest += DispPitch;
	}
	while (--H);
}


// -------------------------------
void IDisplayChunky :: DrawNoTransNoClip_16(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	register _BYTE *TmpPic = (_BYTE *)Src;
	register _BYTE *Dest = DrawSurface + (X << 1) + ( Y * DispPitch );
	W <<= 1;
	do
	{
		Memory::CopyWORD(Dest, TmpPic, W);
		TmpPic += W;
		Dest += DispPitch;
	}
	while (--H);
}


// -------------------------------
void IDisplayChunky :: DrawNoTransNoClip_24(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	//dp("%d %d\n", DispPitch, W);
	register _BYTE *TmpPic = (_BYTE *)Src;
	register _BYTE *Dest = DrawSurface + (X*3) + ( Y * DispPitch );
	W *= 3;
	do
	{
		Memory::Copy(Dest, TmpPic, W);
		TmpPic += W;
		Dest += DispPitch;
	}
	while (--H);
}



// -------------------------------
 void IDisplayChunky :: DrawNoTransNoClip_32(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	register _BYTE *TmpPic = (_BYTE *)Src;
	register _BYTE *Dest = DrawSurface + (X << 2) + ( Y * DispPitch );
	W <<= 2;
	do
	{
		Memory::CopyDWORD(Dest, TmpPic, W);
		TmpPic += W;
		Dest += DispPitch;
	}
	while (--H);
}


// -------------------------------
void IDisplayChunky :: DrawNoTransNoClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	 VALIDATETHIS;
	 ASSERT(Src);
	 if ((!W) || (!H)) return;
	switch (IColorSpace::colorSpace.BitsPerPixel())
	{
	case 8:
		DrawNoTransNoClip_8(Src, X, Y, W, H);
		break;
	case 15:
	case 16:
		DrawNoTransNoClip_16(Src, X, Y, W, H);
		break;
	case 24:
		DrawNoTransNoClip_24(Src, X, Y, W, H);
		break;
	case 32:
		DrawNoTransNoClip_32(Src, X, Y, W, H);
		break;
	default:
		Debug(throw NMOException("IDisplayChunky: invalid BPP");)
		break;
	}
}


// -------------------------------
 void IDisplayChunky :: DrawTransNoClip_8(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_DWORD W2;
	_DWORD Inc;

	_BYTE *Dest = DrawSurface + X + ( Y * DispPitch );
	Inc = DispPitch  - W;
	register _BYTE *TmpDest = (_BYTE *) Dest;
	register _BYTE *TmpPic = (_BYTE *)Src;
	do
	{
		W2 = W;
		do
		{
			if ((*TmpPic)!=(_BYTE)transColor)
				*TmpDest = *TmpPic;
			++TmpDest;
			++TmpPic;
		}
		while (--W2);
		TmpDest += Inc;
	}
	while (--H);
}


// -------------------------------
 void IDisplayChunky :: DrawTransNoClip_16(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_BYTE *Dest;
	_DWORD W2;
	_DWORD Inc;

	Dest = DrawSurface + (X << 1) + ( Y * DispPitch );
	Inc = (DispPitch >> 1) - W;
	register _WORD *TmpDest = (_WORD *) Dest;
	register _WORD *TmpPic = (_WORD *)Src;
	do {
		W2 = W;
		do {
			if ((*TmpPic)!=(_WORD)transColor)
				*TmpDest = *TmpPic;
			++TmpDest;
			++TmpPic;
		}
		while (--W2);
		TmpDest += Inc;
	}
	while (--H);
}



// -------------------------------
 void IDisplayChunky :: DrawTransNoClip_32(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_BYTE *Dest;
	_DWORD W2;
	_DWORD Inc;

	Dest = DrawSurface + (X << 2) + ( Y * DispPitch );
	Inc = (DispPitch >> 2) - W;
	register _DWORD *TmpDest = (_DWORD *) Dest;
	register _DWORD *TmpPic = (_DWORD *)Src;
	do {
		W2 = W;
		do {
			if ((*TmpPic)!=(_DWORD)transColor)
				*TmpDest = *TmpPic;
			++TmpDest;
			++TmpPic;
		}
		while (--W2);
		TmpDest += Inc;
	}
	while (--H);
}


// -------------------------------
 void IDisplayChunky :: DrawTransNoClip_24(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_BYTE *Dest;
	_DWORD W2;
	_DWORD Inc;

	_BYTE trans1 = _BYTE(transColor & 0xff);
	_BYTE trans2 = _BYTE((transColor >> 8) & 0xff);
	_BYTE trans3 = _BYTE((transColor >> 16) & 0xff);

	Dest = DrawSurface + (X * 3) + ( Y * DispPitch );
	register _BYTE *TmpDest = (_BYTE *) Dest;
	register _BYTE *TmpPic = (_BYTE *)Src;
	Inc = ((DispPitch /3) - W) *3;
#ifndef _WIN32
	if ((Inc % 4)) {
		Inc += 4-(Inc % 4);
	}
#endif
	do {
		W2 = W;
		do {
			if ((TmpPic[0] != trans1) && 
				(TmpPic[1] != trans2) && 
				(TmpPic[2] != trans3)) {
				TmpDest[0] = TmpPic[0];
				TmpDest[1] = TmpPic[1];
				TmpDest[2] = TmpPic[2];
			}
			TmpDest += 3;
			TmpPic += 3;
		}
		while (--W2);
		TmpDest += Inc;
	}
	while (--H);
}


// -------------------------------
void IDisplayChunky :: DrawTransNoClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	 VALIDATETHIS;
	 ASSERT(Src);
	 if ((!W) || (!H)) return;
	switch (IColorSpace::colorSpace.BitsPerPixel())
	{
	case 8:
		DrawTransNoClip_8(Src, X, Y, W, H);
		break;
	case 15:
	case 16:
		DrawTransNoClip_16(Src, X, Y, W, H);
		break;
	case 24:
		DrawTransNoClip_24(Src, X, Y, W, H);
		break;
	case 32:
		DrawTransNoClip_32(Src, X, Y, W, H);
		break;
	default:
		Debug(throw NMOException("IDisplayChunky: invalid BPP");)
		break;
	}
}



// -------------------------------
 void IDisplayChunky :: DrawNoTransClip_8(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_LONG YCut;
	_LONG XCut;
	_LONG YEnd;
	_LONG XEnd;
	_LONG Inc;

	if ((Y > YClipEnd) || ((Y + ((_LONG)H)) <= YClipStart) || ((X + ((_LONG)W)) <= XClipStart) || (X > XClipEnd))
      return;

	YCut = YClipStart - Y;
	XCut = XClipStart - X;
	YEnd = YClipEnd - Y + 1;
	XEnd = XClipEnd - X + 1;

	register _BYTE *TmpPic = (_BYTE *)Src;
	Inc = W;

	if (YCut > 0)
	{
		TmpPic += YCut * W;
		H -= YCut;
	}
	if (YEnd < (_LONG)H)
	{
		H = YEnd;
	}
	if (XCut > 0)
	{
		TmpPic += XCut;
		W -= XCut;
	}
	if (XEnd < (_LONG)W)
	{
		W = XEnd;
	}

	register _BYTE *Dest = DrawSurface + ( MAX(X, XClipStart) ) + ( MAX(Y, YClipStart) * DispPitch );
	do {
		Memory::Copy(Dest, TmpPic, W);
		TmpPic += Inc;
		Dest += DispPitch;
	}
	while (--H);
}




// -------------------------------
 void IDisplayChunky :: DrawNoTransClip_16(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_LONG YCut;
	_LONG XCut;
	_LONG YEnd;
	_LONG XEnd;
	_LONG Inc;

	if ((Y > YClipEnd) || ((Y + ((_LONG)H)) < YClipStart) || ((X + ((_LONG)W)) < XClipStart) || (X > XClipEnd))
      return;

	YCut = YClipStart - Y;
	XCut = XClipStart - X;
	YEnd = YClipEnd - Y + 1;
	XEnd = XClipEnd - X + 1;
	Inc = W;

	register _BYTE *TmpPic = (_BYTE *)Src;

	if (YCut > 0)
	{
		TmpPic += YCut * (W << 1);
		H -= YCut;
	}
	if (YEnd < (_LONG)H)
	{
		H = YEnd;
	}
	if (XCut > 0)
	{
		TmpPic += (XCut << 1);
		W -= XCut;
	}
	if (XEnd < (_LONG)W)
	{
		W = XEnd;
	}

	register _BYTE *Dest = DrawSurface + ( MAX(X, XClipStart) << 1 ) + ( MAX(Y, YClipStart) * DispPitch );
	W <<= 1;
	Inc <<= 1;
	do {
		Memory::CopyWORD(Dest, TmpPic, W);
		TmpPic += Inc;
		Dest += DispPitch;
	}
	while (--H);
}




// -------------------------------
 void IDisplayChunky :: DrawNoTransClip_32(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_LONG YCut;
	_LONG XCut;
	_LONG YEnd;
	_LONG XEnd;
	_LONG Inc;

	if ((Y > YClipEnd) || ((Y + ((_LONG)H)) <= YClipStart) || ((X + ((_LONG)W)) <= XClipStart) || (X > XClipEnd))
      return;

	YCut = YClipStart - Y;
	XCut = XClipStart - X;
	YEnd = YClipEnd - Y + 1;
	XEnd = XClipEnd - X + 1;
	Inc = W;

	register _BYTE *TmpPic = (_BYTE *)Src;

	if (YCut > 0)
	{
		TmpPic += YCut * (W << 2);
		H -= YCut;
	}
	if (YEnd < (_LONG)H)
	{
		H = YEnd;
	}
	if (XCut > 0)
	{
		TmpPic += (XCut << 2);
		W -= XCut;
	}
	if (XEnd < (_LONG)W)
	{
		W = XEnd;
	}

	register _BYTE *Dest = DrawSurface + ( MAX(X, XClipStart) << 2 ) + ( MAX(Y, YClipStart) * DispPitch );
	W <<= 2;
	Inc <<= 2;
	do {
		Memory::Copy(Dest, TmpPic, W);
		TmpPic += Inc;
		Dest += DispPitch;
	}
	while (--H);
}


// -------------------------------
 void IDisplayChunky :: DrawNoTransClip_24(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_LONG YCut;
	_LONG XCut;
	_LONG YEnd;
	_LONG XEnd;
	_LONG Inc;

	if ((Y > YClipEnd) || ((Y + ((_LONG)H)) <= YClipStart) || ((X + ((_LONG)W)) <= XClipStart) || (X > XClipEnd))
      return;

	YCut = YClipStart - Y;
	XCut = XClipStart - X;
	YEnd = YClipEnd - Y + 1;
	XEnd = XClipEnd - X + 1;

	register _BYTE *TmpPic = (_BYTE *)Src;
	Inc = W;

	if (YCut > 0)
	{
		TmpPic += YCut * W *3;
		H -= YCut;
	}
	if (YEnd < (_LONG)H)
	{
		H = YEnd;
	}
	if (XCut > 0)
	{
		TmpPic += XCut *3;
		W -= XCut;
	}
	if (XEnd < (_LONG)W)
	{
		W = XEnd;
	}

	register _BYTE *Dest = DrawSurface + ( MAX(X, XClipStart)*3 ) + ( MAX(Y, YClipStart) * DispPitch );
	W *= 3;
	Inc *= 3;
	do {
		Memory::CopyDWORD(Dest, TmpPic, W);
		TmpPic += Inc;
		Dest += DispPitch;
	}
	while (--H);
}


// -------------------------------
 void IDisplayChunky :: DrawNoTransClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	 VALIDATETHIS;
	 ASSERT(Src);
	 if ((!W) || (!H)) return;
	switch (IColorSpace::colorSpace.BitsPerPixel())
	{
	case 8:
		DrawNoTransClip_8(Src, X, Y, W, H);
		break;
	case 15:
	case 16:
		DrawNoTransClip_16(Src, X, Y, W, H);
		break;
	case 24:
		DrawNoTransClip_24(Src, X, Y, W, H);
		break;
	case 32:
		DrawNoTransClip_32(Src, X, Y, W, H);
		break;
	default:
		Debug(throw NMOException("IDisplayChunky: invalid BPP");)
		break;
	}
}




// -------------------------------
 void IDisplayChunky :: DrawTransClip_8(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_LONG YCut;
	_LONG XCut;
	_LONG YEnd;
	_LONG XEnd;
	_LONG Inc;
	_LONG W2;
	_LONG Inc2;

	if ((Y > YClipEnd) || ((Y + ((_LONG)H)) <= YClipStart) || ((X + ((_LONG)W)) <= XClipStart) || (X > XClipEnd))
      return;

	YCut = YClipStart - Y;
	XCut = XClipStart - X;
	YEnd = YClipEnd - Y + 1;
	XEnd = XClipEnd - X + 1;
	Inc = W;

	_BYTE *Dest = (_BYTE *) DrawSurface + ( MAX(X, XClipStart)) + ( MAX(Y, YClipStart) * DispPitch );
	register _BYTE *TmpDest = (_BYTE *) Dest;
	register _BYTE *TmpPic = (_BYTE *)Src;

	if (YCut > 0) {
		TmpPic += YCut * W;
		H -= YCut;
	}
	if (YEnd < (_LONG)H) {
		H = YEnd;
	}
	if (XCut > 0) {
		TmpPic += XCut;
		W -= XCut;
	}
	if (XEnd < (_LONG)W) {
		W = XEnd;
	}

	Inc -= W;
	Inc2 = DispPitch - W;
	do {
		W2 = W;
		do {
			if ((*TmpPic)!=(_BYTE)transColor)
				*TmpDest = *TmpPic;
			++TmpDest;
			++TmpPic;
		}
		while (--W2);
		TmpPic += Inc;
		TmpDest += Inc2;
	}
	while (--H);
}


// -------------------------------
 void IDisplayChunky :: DrawTransClip_16(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_LONG YCut;
	_LONG XCut;
	_LONG YEnd;
	_LONG XEnd;
	_LONG Inc;
	_LONG W2;
	_LONG Inc2;

	if ((Y > YClipEnd) || ((Y + ((_LONG)H)) <= YClipStart) || ((X + ((_LONG)W)) <= XClipStart) || (X > XClipEnd)) {
      return;
	}

	YCut = YClipStart - Y;
	XCut = XClipStart - X;
	YEnd = YClipEnd - Y + 1;
	XEnd = XClipEnd - X + 1;
	Inc = W;

	_BYTE *Dest = DrawSurface + ( MAX(X, XClipStart) << 1 ) + ( MAX(Y, YClipStart) * DispPitch );
	register _WORD *TmpPic = (_WORD *)Src;
	register _WORD *TmpDest = (_WORD *)Dest;

	if (YEnd < (_LONG)H) {
		H = YEnd;
	}
	if (YCut > 0) {
		TmpPic += YCut * W;
		H -= YCut;
	}
	if (XEnd < (_LONG)W) {
		W = XEnd;
	}
	if (XCut > 0) {
		TmpPic += XCut;
		W -= XCut;
	}

	Inc -= W;
	Inc2 = (DispPitch >> 1) - W;
	do {
		W2 = W;
		do {
			if ((*TmpPic)!=(_WORD)transColor)
				*TmpDest = *TmpPic;
			++TmpDest;
			++TmpPic;
		}
		while (--W2);
		TmpPic += Inc;
		TmpDest += Inc2;
	}
	while (--H);
}



// -------------------------------
 void IDisplayChunky :: DrawTransClip_32(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_LONG YCut;
	_LONG XCut;
	_LONG YEnd;
	_LONG XEnd;
	_LONG Inc;
	_LONG W2;
	_LONG Inc2;

	if ((Y > YClipEnd) || ((Y + ((_LONG)H)) <= YClipStart) || ((X + ((_LONG)W)) <= XClipStart) || (X > XClipEnd))
      return;

	YCut = YClipStart - Y;
	XCut = XClipStart - X;
	YEnd = YClipEnd - Y + 1;
	XEnd = XClipEnd - X + 1;
	Inc = W;

	_BYTE *Dest = DrawSurface + ( MAX(X, XClipStart) << 2 ) + ( MAX(Y, YClipStart) * DispPitch );
	register _DWORD *TmpPic = (_DWORD *)Src;
	register _DWORD *TmpDest = (_DWORD *)Dest;

	if (YEnd < (_LONG)H) {
		H = YEnd;
	}
	if (YCut > 0) {
		TmpPic += YCut * W;
		H -= YCut;
	}
	if (XEnd < (_LONG)W) {
		W = XEnd;
	}
	if (XCut > 0) {
		TmpPic += XCut;
		W -= XCut;
	}

	Inc -= W;
	Inc2 = (DispPitch >> 2) - W;
	do {
		W2 = W;
		do {
			if ((*TmpPic)!=(_DWORD)transColor)
				*TmpDest = *TmpPic;
			++TmpDest;
			++TmpPic;
		}
		while (--W2);
		TmpPic += Inc;
		TmpDest += Inc2;
	}
	while (--H);
}


// -------------------------------
 void IDisplayChunky :: DrawTransClip_24(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	_LONG YCut;
	_LONG XCut;
	_LONG YEnd;
	_LONG XEnd;
	_LONG Inc;
	_LONG W2;
	_LONG Inc2;

	if ((Y > YClipEnd) || ((Y + ((_LONG)H)) <= YClipStart) || ((X + ((_LONG)W)) <= XClipStart) || (X > XClipEnd))
      return;

	YCut = YClipStart - Y;
	XCut = XClipStart - X;
	YEnd = YClipEnd - Y + 1;
	XEnd = XClipEnd - X + 1;
	Inc = W * 3;

	_BYTE *Dest = DrawSurface + ( MAX(X, XClipStart) * 3 ) + ( MAX(Y, YClipStart) * DispPitch );
	register _BYTE *TmpPic = (_BYTE *)Src;
	register _BYTE *TmpDest = (_BYTE *)Dest;

	if (YEnd < (_LONG)H) {
		H = YEnd;
	}
	if (YCut > 0) {
		TmpPic += (YCut * W) * 3;
		H -= YCut;
	}
	if (XEnd < (_LONG)W) {
		W = XEnd;
	}
	if (XCut > 0) {
		TmpPic += XCut * 3;
		W -= XCut;
	}

	_BYTE trans1 = _BYTE(transColor & 0xff);
	_BYTE trans2 = _BYTE((transColor >> 8) & 0xff);
	_BYTE trans3 = _BYTE((transColor >> 16) & 0xff);

	Inc -= W * 3;
	Inc2 = (DispPitch - (W * 3));
#ifndef _WIN32
	if ((Inc2 % 4)) {
		Inc2 += 4-(Inc2 % 4);
	}
#endif
	do {
		W2 = W;
		do {
			if ((TmpPic[0] != trans1) ||
				(TmpPic[1] != trans2) || 
				(TmpPic[2] != trans3)) {
				TmpDest[0] = TmpPic[0];
				TmpDest[1] = TmpPic[1];
				TmpDest[2] = TmpPic[2];
			}
			TmpDest += 3;
			TmpPic += 3;
		}
		while (--W2);
		TmpPic += Inc;
		TmpDest += Inc2;
	}
	while (--H);
}


// -------------------------------
 void IDisplayChunky :: DrawTransClip(const void *Src, _LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	 VALIDATETHIS;
	 ASSERT(Src);
	 if ((!W) || (!H)) return;
	switch (IColorSpace::colorSpace.BitsPerPixel())
	{
	case 8:
		DrawTransClip_8(Src, X, Y, W, H);
		break;
	case 15:
	case 16:
		DrawTransClip_16(Src, X, Y, W, H);
		break;
	case 24:
		DrawTransClip_24(Src, X, Y, W, H);
		break;
	case 32:
		DrawTransClip_32(Src, X, Y, W, H);
		break;
	default:
		Debug(throw NMOException("IDisplayChunky: invalid BPP");)
		break;
	}
}


void IDisplayChunky :: GetPixel (_LONG, _LONG, IRGBColor&)
{
	ASSERT(FALSE);
}



// -------------------------------
void IDisplayChunky :: DrawPixel (_LONG X, _LONG Y)
{
	if ((Y > YClipEnd) || (Y < YClipStart) ||
		(X < XClipStart) || (X > XClipEnd))
      		return;

	_BYTE *Dest;

	switch (IColorSpace::colorSpace.BitsPerPixel())
	{
	case 8:
		Dest = DrawSurface + X + (Y*DispPitch);
		*Dest = (_BYTE) penColor;
		break;
	case 15:
	case 16:
		{
			Dest = DrawSurface + (X * 2) + ( Y * DispPitch );
			//Dest = DrawSurface + (X << 1) + ( Y * DispPitch );
			_WORD *TmpDest = (_WORD*) Dest;
			*TmpDest = (_WORD) penColor;
		}
		break;
	case 24:
		{
			Dest = DrawSurface + (X * 3) + ( Y * (DispPitch ));
			*Dest = (_BYTE) penColor;
			Dest++;
			*Dest = (_BYTE) (penColor >> 8);
			Dest++;
			*Dest = (_BYTE) (penColor >> 16);
		}
		break;
	case 32:
		{
			Dest = DrawSurface + (X << 2) + ( Y * DispPitch );
			_DWORD *TmpDest = (_DWORD*) Dest;
			*TmpDest = penColor;
		}
		break;
	default:
		Debug(throw NMOException("IDisplayChunky: invalid BPP");)
		break;
	}
}



// -------------------------------
void IDisplayChunky :: DrawLine (_LONG x1, _LONG y1, _LONG x2, _LONG y2)
{
  register _LONG d, x, y, ax, ay, sx, sy, dx, dy;

  dx = x2-x1; ax = ABS(dx)<<1; 
  sx = SIGN(dx); x = x1;
  dy = y2-y1; ay = ABS(dy)<<1; 
  sy = SIGN(dy); y = y1;

  if (ax>ay) {					/* x dominant */
    //d = ay-(ax>>1);
    d = ay-(ax/2);
    for (;;) {
		DrawPixel(x, y);
      if (x==x2)
	break;
      if (d>=0) {
	y += sy; d -= ax;
      }
      x += sx; d += ay;
    }
  }
  else {					/* y dominant */
    //d = ax-(ay>>1);
    d = ax-(ay/2);
    for (;;) {
		DrawPixel(x, y);
      if (y==y2) 
	break;
      if (d>=0) {
	x += sx; d -= ay;
      }
      y += sy; d += ax;
    }
  }
}


// -------------------------------
void IDisplayChunky :: Clear()
{
	if (DispHeight == 0) { return; }
	
	switch (IColorSpace::colorSpace.BitsPerPixel())
	{
	case 8:
		Memory::Fill(DrawSurface, (_BYTE)penColor, DispPitch * DispHeight);
		break;
	case 15:
	case 16:
		{
			register _WORD *dest = (_WORD*)DrawSurface;
			register _DWORD idx;
			for (idx = 0; idx < DispWidth; idx ++) {
				dest[idx] = (_WORD)penColor;
			}
			_DWORD w = DispWidth * 2;
			for (idx = 1; idx < DispHeight; idx++) {
				Memory::Copy(DrawSurface + idx * DispPitch,
							 DrawSurface,
							 w);
			}
		}
		break;
	case 24:
		{
			_BYTE b1 = _BYTE(penColor & 0xff);
			_BYTE b2 = _BYTE((penColor >> 8) & 0xff);
			_BYTE b3 = _BYTE((penColor >> 16) & 0xff);
			register _DWORD idx;
			for (idx = 0; idx < (DispWidth*3); idx += 3) {
				DrawSurface[idx] = b1;
				DrawSurface[idx+1] = b2;
				DrawSurface[idx+2] = b3;
			}
			register _DWORD w = DispWidth * 3;
			for (idx = 1; idx < DispHeight; idx++) {
				Memory::Copy(DrawSurface + idx * DispPitch,
							 DrawSurface,
							 w);
			}
		}
		break;
	case 32:
		{
			register _DWORD *dest = (_DWORD*)DrawSurface;
			register _DWORD idx;
			for (idx = 0; idx < DispWidth; idx ++) {
				dest[idx] = (_DWORD)penColor;
			}
			_DWORD w = DispWidth * 4;
			for (idx = 1; idx < DispHeight; idx++) {
				Memory::Copy(DrawSurface + idx * DispPitch,
							 DrawSurface,
							 w);
			}
		}
		break;
	default:
		Debug(throw NMOException("IDisplayChunky: invalid BPP");)
		break;
	}
}


// -------------------------------
void IDisplayChunky :: DrawRect(_LONG X, _LONG Y, _DWORD W, _DWORD H)
{
	if ((Y > YClipEnd) || (X > XClipEnd)) {
		return;
	}
	for (_DWORD i = 0; i < H; i++) DrawHLine(X, Y + i, W);
}


// -------------------------------
void IDisplayChunky :: DrawHLine (_LONG X, _LONG Y, _DWORD W)
{
	if ((Y < YClipStart) || (Y > YClipEnd) || 
		(X > XClipEnd) || ((X+_LONG(W)) <= XClipStart))
		return;
	const _LONG XCut = XClipStart - X;
	if (XCut > 0) {
		X = XClipStart;
		W -= XCut;
	}
	if (XClipEnd < (X + _LONG(W) - 1)) {
		W = XClipEnd - X + 1;
	}

	register _BYTE *Dest;
	switch (IColorSpace::colorSpace.BitsPerPixel())
	{
	case 8:
		Dest = DrawSurface + X + (Y*DispPitch);
		Memory::Fill(Dest, (_BYTE)penColor, W);
		break;
	case 15:
	case 16:
		{
			Dest = DrawSurface + (X*2) + (Y*DispPitch);
			register _WORD *dest = (_WORD*)Dest;
			for (_DWORD i=0; i<W; i++) { dest[i] = (_WORD)penColor; }
		}
		break;
	case 24:
		{
			Dest = DrawSurface + (X*3) + (Y*DispPitch);
			_BYTE b1 = _BYTE(penColor & 0xff);
			_BYTE b2 = _BYTE((penColor >> 8) & 0xff);
			_BYTE b3 = _BYTE((penColor >> 16) & 0xff);
			for (_DWORD i=0; i<(W*3); i+=3) {
				Dest[i] = b1;
				Dest[i+1] = b2;
				Dest[i+2] = b3;
			}
		}
		break;
	case 32:
		{
			Dest = DrawSurface + (X*4) + (Y*DispPitch);
			register _DWORD *dest = (_DWORD*)Dest;
			for (_DWORD i=0; i<W; i++) {
				dest[i] = penColor;
			}	
		}
		break;
	default:
		Debug(throw NMOException("IDisplayChunky: invalid BPP");)
		break;
	}
}



// -------------------------------
void IDisplayChunky :: DrawCircle (_LONG lgX, _LONG lgY, _DWORD dwB, _BOOL bFILL, _DWORD dwSTEP)
{
	ASSERTMSG(dwSTEP > 0, "IDisplayChunky :: DrawCircle: pixel step must be > 0");
	_DWORD radqu;
	_DWORD xmax;
	register _DWORD x;
	register _DWORD y;
	_LONG x0, y0;
	_LONG u1, u2, u3, u4;
	_LONG v1, v2, v3, v4;

	x0 = lgX;
	y0 = lgY;
	radqu = dwB * dwB;
	xmax = _DWORD(dwB / sqrt(2));
	if (bFILL) {
		for (x = 0; x <= xmax; x += dwSTEP) {
			y = _DWORD(sqrt(radqu - x * x));
			u1 = x0 - x; v1 = y0 - y;
			u2 = x0 + x; v2 = y0 + y;
			u3 = x0 - y; v3 = y0 - x;
			u4 = x0 + y; v4 = y0 + x;
			DrawLine(u1, v1, u1, v2);
			DrawLine(u3, v3, u3, v4);
			DrawLine(u2, v2, u2, v1);
			DrawLine(u4, v4, u4, v3);
		}
	}
	else {
		for (x = 0; x <= xmax; x += dwSTEP) {
			y = _DWORD(sqrt(radqu - x * x));
			u1 = x0 - x; v1 = y0 - y;
			u2 = x0 + x; v2 = y0 + y;
			u3 = x0 - y; v3 = y0 - x;
			u4 = x0 + y; v4 = y0 + x;
			DrawPixel (u1, v1);
			DrawPixel (u1, v2);
			DrawPixel (u2, v1);
			DrawPixel (u2, v2);
			DrawPixel (u3, v3);
			DrawPixel (u3, v4);
			DrawPixel (u4, v3);
			DrawPixel (u4, v4);
		}
	}
}
