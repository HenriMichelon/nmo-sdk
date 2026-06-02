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
#ifndef _NMO_INTERFACE_COLORS_H_
#define _NMO_INTERFACE_COLORS_H_

namespace NMO {

/*!
\brief		Display colors types, convertions & remapping


\author		(c) 2000-2001 Henri Michelon 
\version	$Id: IColorSpace.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------



class IColorSpace: public NMOObject
{
public:
	static IColorSpace colorSpace;

	virtual ~IColorSpace();

	/*! Return the current indexed palette */
	IPalette &Palette();

	/*!	Remap bits to current pixel format & 'pal' palette colors.
		\param void*	: memory location of the bitmap bits
		\param IPalette	: colors to remap to (if bpp == 1)
		\param _DWORD	: size of the bitmap (in bytes)
		\param _DWORD	: bytes per pixel
		\return Bytes Per Pixel of the remaped bitmap
	*/
	_DWORD Remap(void *&, const IPalette*, _DWORD, _DWORD);

	/*! convert RGB to dependent color.
		\param const RGBColor: color to convert
	*/
	_DWORD Color(const IRGBColor &);

	/*! convert RGB to dependent color
		\param _BYTE: red
		\param _BYTE: green
		\param _BYTE: blue
	*/
	_DWORD Color(_BYTE, _BYTE, _BYTE);

	/*! convert dependent color to RGB
		\param _DWORD: color
		\param _BYTE&: red (output)
		\param _BYTE&: green (output)
		\param _BYTE&: blue (output)
	*/
	void Color(_DWORD, _BYTE &, _BYTE &, _BYTE &);

	/*! convert dependent color to RGB
		\param _DWORD	: dependent color
		\param RGBColor: RGB color (output)
	*/
	void Color(_DWORD, IRGBColor &);

	/*! get the color depth of the display in BITS per pixel */
	_DWORD  BitsPerPixel();

	/*! get the color depth of the display in BYTES per pixel */
	_DWORD BytesPerPixel();

	/*! Set the pixel format.
	  	Install a default color palette if the color space
		palette is empty.
		\param	_BYTE:	number of bits per pixel */
	void SetBitsPerPixel(_DWORD);

	/*! Set the pixel color masks for Remap() function.
		Currently, only the following color order is supported: RGB
		\param _DWORD	: red mask
		\param _DWORD	: green mask
		\param _DWORD	: blue mask
	*/
	void SetColorMasks(_DWORD, _DWORD, _DWORD);

private:
	typedef struct
	{
		_DWORD bitmask;
		_DWORD shift;
		_DWORD dither;
		_DWORD pos;
	} PixelRemapMask;

	IPalette palette;

	PixelRemapMask redMask;
	PixelRemapMask blueMask;
	PixelRemapMask greenMask;

	_DWORD bitsPerPixel;
	_DWORD bytesPerPixel;

	_WORD CalcPos(_DWORD);
	void CalcShift(PixelRemapMask &,PixelRemapMask &,PixelRemapMask &);
};


inline _DWORD IColorSpace :: Color(const IRGBColor &C)
{ return Color(C.red, C.green, C.blue); }

inline void IColorSpace :: Color(_DWORD I, IRGBColor &C)
{ Color(I, C.red, C.green, C.blue); }

}

#endif
