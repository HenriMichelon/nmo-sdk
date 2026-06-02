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

$Id: IColorSpace.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/interface/IRect.hpp>
#include <nmo/interface/IRGBColor.hpp>
#include <nmo/interface/IPalette.hpp>
#include <nmo/interface/IColorSpace.hpp>
#include <nmo/interface/DefaultPal.h>
using namespace NMO;


#define _m(x) this->x


// -------------------------------
IColorSpace IColorSpace :: colorSpace;


// -------------------------------
IColorSpace::~IColorSpace()
{
	_m(palette).Clear();
}


// -------------------------------
IPalette &IColorSpace::Palette()
{
	return _m(palette);
}


// -------------------------------
void IColorSpace::SetBitsPerPixel(_DWORD BPP)
{
	_m(bitsPerPixel) = BPP;
	switch (_m(bitsPerPixel))
	{
	case 8:
		if (!_m(palette).Count()) {
			_m(palette).MapRGB((_BYTE*)DefaultPal);
		}
		_m(bytesPerPixel) = 1;
		break;
	case 15:
	case 16:
		_m(bytesPerPixel) = 2;
		break;
  	case 24:
		_m(bytesPerPixel) = 3;
		break;
	case 32:
		_m(bytesPerPixel) = 4;
		break;
	default:
		dprintf("IColorSpace: invalid bitperpixel mode\n");
		break;
	}
}


// -------------------------------
_DWORD IColorSpace :: Remap(void *&Pic, const IPalette *Pale, _DWORD Size, _DWORD BPP)
{
	switch (BPP) {
	case 1:
		ASSERT(Pale);
		if (_m(bitsPerPixel) == 8) {
			register _WORD c1, c2;
			_LONG moy, m2;
			IPalette::Tbest   *best_red;
			IPalette::Tbest   *best_green;
			IPalette::Tbest   *best_blue;
			IPalette::Tbest   bestfit;
			_BYTE	*TmpPic = (_BYTE*)Pic;

			best_red = new IPalette::Tbest[256]; 
			best_green = new IPalette::Tbest[256]; 
			best_blue = new IPalette::Tbest[256]; 

			//dp("permutation palettes...\n");
			for (c1=0; c1<Pale->Count(); c1++)
			{
				for (c2=0; c2<256; c2++)
				{
					if ((*Pale)[c1].red>_m(palette)[c2].red)
						best_red[c1][c2] = (*Pale)[c1].red - _m(palette)[c2].red;
					else
						best_red[c1][c2] = _m(palette)[c2].red - (*Pale)[c1].red;
					if ((*Pale)[c1].green>_m(palette)[c2].green)
						best_green[c1][c2] = (*Pale)[c1].green - _m(palette)[c2].green;
					else
						best_green[c1][c2] = _m(palette)[c2].green - (*Pale)[c1].green;
					if ((*Pale)[c1].blue>_m(palette)[c2].blue)
						best_blue[c1][c2] = (*Pale)[c1].blue - _m(palette)[c2].blue;
					else
						best_blue[c1][c2] = _m(palette)[c2].blue - (*Pale)[c1].blue;
				}
			}
			//dp("approximation...\n");
			for (c1=0; c1<Pale->Count(); c1++)
			{
				moy = 100; // max == 63-0
				for (c2=0; c2<256; c2++)
				{
					m2 = (best_red[c1][c2]*2+best_green[c1][c2]*3+best_blue[c1][c2]*4)/9;
					if (m2<moy)
					{
						moy=m2;
						bestfit[c1] = (_BYTE) c2;
					}
				}
			}
			//dp ("matching image...\n");
			for (_DWORD i=0; i<Size; i++)
				TmpPic[i] = bestfit[TmpPic[i]];

			delete []best_blue;
			delete []best_green;
			delete []best_red;
		}
		else // 16, 24 & 32 bpp modes
		{
			IRGBColor 	Col;
			_DWORD cwidth = _m(bytesPerPixel);
			_BYTE *TmpByte = new _BYTE[Size*cwidth];
			_BYTE *TmpPic = (_BYTE*)Pic;
			_WORD *TmpWord= (_WORD*) TmpByte;
			_DWORD *TmpDword = (_DWORD*) TmpByte;
			if (!TmpByte) {
				dprintf("IColorSpace::Remap(): can't allocate new buffer!\n");
				return 0;
			}

			for (_DWORD i = 0; i < Size; i++) {
				Col = (*Pale)[TmpPic[i]];
				switch (_m(bitsPerPixel)) {
				case 15:
				case 16:
					{
						TmpWord[i] = (_WORD) Color(Col.red, Col.green, Col.blue);
						//dprintf("%d %d %d=%d\n", Col.red, Col.green, Col.blue, TmpWord[i]);
					}
					break;
				case 24:
					{
						_DWORD col;
						col = Color(Col.red, Col.green, Col.blue);
						TmpByte[i*cwidth] = (_BYTE)col;
						TmpByte[i*cwidth+1] = (_BYTE)(col>>8);
						TmpByte[i*cwidth+2] = (_BYTE)(col>>16);
					}
					break;
				case 32:
					{
						TmpDword[i] = Color(Col.red, Col.green, Col.blue);
					}
					break;
				default:
					dprintf("IColorSpace::Remap(): Invalid BPP mode");
					ASSERT(FALSE);
					break;
				}
			}
			delete [](_BYTE*)Pic;
			Pic = (void *)TmpByte;
		}
		break;
	case 2:
		return 0;
		break;
	case 3:
		switch (_m(bytesPerPixel)) {
		case 1:
			break;
		case 2:
			{
				_WORD* TmpDst = new _WORD[Size * _m(bytesPerPixel)];
				_BYTE* TmpSrc = (_BYTE*)Pic;
				for (_DWORD idx = 0; idx < Size; idx ++) {
					TmpDst[idx] =	((TmpSrc[idx*3] >> 2) << _m(redMask).pos) +
									((TmpSrc[idx*3 + 1] >> 2) << _m(greenMask).pos) +
									((TmpSrc[idx*3 + 2] >> 2) << _m(blueMask).pos);
				}
				delete [](_BYTE*)Pic;
				Pic = (_PTR)TmpDst;
			}
			break;
		case 3:
			break;
		case 4:
			break;
		}
		break;
	case 4:
		return 0;
		break;
	}
	return _m(bytesPerPixel);
}


// -------------------------------
_DWORD IColorSpace :: Color(_BYTE R, _BYTE G, _BYTE B)
{
	if (_m(bitsPerPixel) > 8) {
		return ( ((R/_m(redMask).dither) << _m(redMask).pos)) |
				( ((G/_m(greenMask).dither) << _m(greenMask).pos)) |
				( ((B/_m(blueMask).dither) << _m(blueMask).pos));
	}
	else {
	    _WORD	c2, moy, m2;
		_BYTE	bestfit = 0;
		IPalette::Tbest   best_red, best_green, best_blue;

		for (c2=0; c2<256; c2++) {
			if (R>_m(palette)[c2].red)
				best_red[c2] = (_BYTE) R - _m(palette)[c2].red;
	        else
		        best_red[c2] = (_BYTE) _m(palette)[c2].red - R;
			if (G>_m(palette)[c2].green)
				best_green[c2] = (_BYTE) G - _m(palette)[c2].green;
	        else
		        best_green[c2] = (_BYTE) _m(palette)[c2].green - G;
			if (B>_m(palette)[c2].blue)
				best_blue[c2] = (_BYTE) B - _m(palette)[c2].blue;
	         else
		        best_blue[c2] = (_BYTE) _m(palette)[c2].blue - B;
		}
		moy = 100; // max == 63-0
        for (c2=0; c2<256; c2++) {
		    m2 = (_WORD) (best_red[c2]*2+best_green[c2]*3+best_blue[c2]*4)/9;
			if (m2<moy) {
	            moy=m2;
		        bestfit = (_BYTE) c2;
			}
        }
		return bestfit;
	}
}


// -------------------------------
void IColorSpace :: Color(_DWORD Col, _BYTE &R, _BYTE &G, _BYTE &B)
{
	if (_m(bitsPerPixel) > 8) {
		R = _BYTE (( (Col & _m(redMask).bitmask) / _m(redMask).shift ) * _m(redMask).dither);
		G = _BYTE (( (Col & _m(greenMask).bitmask) / _m(greenMask).shift ) * _m(greenMask).dither);
		B = _BYTE (( (Col & _m(blueMask).bitmask) / _m(blueMask).shift ) * _m(blueMask).dither);
	}
	else {
		R = _m(palette)[Col].red;
		G = _m(palette)[Col].green;
		B = _m(palette)[Col].blue;
	}
}


// -------------------------------
_WORD IColorSpace :: CalcPos(_DWORD ww)
{
	_BYTE i = 0;
	while ( (ww & 0x1)==0 )	{
		i++;
		ww >>= 1;
	}
	return i;
}


// -------------------------------
void IColorSpace :: CalcShift(IColorSpace::PixelRemapMask &M1, 
							IColorSpace::PixelRemapMask &M2,
							IColorSpace::PixelRemapMask &M3)
{
	M1.shift = (~(M1.bitmask-1)) & 0x7F;
	if (!M1.shift)
		M1.shift = (~(M1.bitmask-1)) & 0x7FFF;
	if (!M1.shift)
		M1.shift = (~(M1.bitmask-1)) & 0x7FFFFF;
	if (!M1.shift)
		M1.shift = (~(M1.bitmask-1)) & 0x7FFFFFFF;

	M2.shift = ( (~M1.bitmask) & (~(M2.bitmask-1)) )  & 0x7F;
	if (!M2.shift)
		M2.shift = ( (~M1.bitmask) & (~(M2.bitmask-1))) & 0x7FFF;
	if (!M2.shift)
		M2.shift = ( (~M1.bitmask) & (~(M2.bitmask-1))) & 0x7FFFFF;
	if (!M2.shift)
		M2.shift = ( (~M1.bitmask) & (~(M2.bitmask-1))) & 0x7FFFFFFF;

	M3.shift = ( (~(M1.bitmask | M2.bitmask)) & (~(M3.bitmask-1)) )  & 0x7F;
	if (!M3.shift)
		M3.shift = ( (~(M1.bitmask | M2.bitmask)) & (~(M3.bitmask-1))) & 0x7FFF;
	if (!M3.shift)
		M3.shift = ( (~(M1.bitmask | M2.bitmask)) & (~(M3.bitmask-1))) & 0x7FFFFF;
	if (!M3.shift)
		M3.shift = ( (~(M1.bitmask | M2.bitmask)) & (~(M3.bitmask-1))) & 0x7FFFFFFF;

}


// -------------------------------
void IColorSpace :: SetColorMasks(_DWORD R, _DWORD G, _DWORD B)
{
	_m(redMask).bitmask = R;
	_m(greenMask).bitmask = G;
	_m(blueMask).bitmask = B;
	_m(CalcShift)(_m(redMask), _m(greenMask), _m(blueMask));
	_m(redMask).dither = 256 / ( (_m(redMask).bitmask / _m(redMask).shift ) + 1 );
	_m(greenMask).dither = 256 / ( (_m(greenMask).bitmask / _m(greenMask).shift ) + 1 );
	_m(blueMask).dither = 256 / ( (_m(blueMask).bitmask / _m(blueMask).shift ) + 1 );
	_m(redMask).pos = _m(CalcPos)(_m(redMask).bitmask);
	_m(greenMask).pos = _m(CalcPos)(_m(greenMask).bitmask);
	_m(blueMask).pos = _m(CalcPos)(_m(blueMask).bitmask);
}


// -------------------------------
_DWORD IColorSpace :: BytesPerPixel()
{ 
	return _m(bytesPerPixel); 
}


// -------------------------------
_DWORD IColorSpace :: BitsPerPixel()
{ 
	return _m(bitsPerPixel); 
}

