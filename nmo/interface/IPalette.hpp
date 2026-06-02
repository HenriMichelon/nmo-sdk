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
#ifndef _IPalette_H_
#define _IPalette_H_

namespace NMO {

/*!
\brief		RGB palette & colors entries
	

\author		(c) 1998-2001 Henri Michelon 
\version	$Id: IPalette.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------



class IPalette: public NMOObject
{
public:
	// Used for IDisplay::Remap() for color matching
	typedef _BYTE Tbest[256];

	/*! Create a new palette
		\param _DWORD	: size of the palette, in number of entries
	*/
	IPalette(_DWORD);

	/*! Create an empty palette */
	IPalette();

	/*! Copy an existing palette */
	IPalette(IPalette&);

	virtual ~IPalette();

	/*! Destroy the palette datas */		
	void Clear();
	
	/*! Recreate the palette with a new size 
		\param _DWORD	: size of the palette, in number of entries
	*/		
	void Reset(_DWORD);

	/*! Clone a palette:
		The two palettes (new and old) share the
		same colors array
	*/
	IPalette *Clone();

	/*! Copy a palette:
		 Copy all, colors array included
	*/
	IPalette *Copy();

	/*! Return the color at a specific index
		\param _DWORD	: index, 0 based
	*/
	inline const IRGBColor &operator[](_DWORD) const;

	/*! Return the color at a specific index
		\param _DWORD	: index, 0 based
	*/
	inline const IRGBColor &Get(_DWORD) const;

	/*! Set the color at a specific index
		\param _DWORD	: index, 0 based
		\param IRGBColor: new color to set
	*/
	void Set(_DWORD, const IRGBColor&);

	/*! Set the color at a specific index
		\param _DWORD: index, 0 based
		\param _BYTE: red composant
		\param _BYTE: green composant
		\param _BYTE: blue composant
	*/
	void Set(_DWORD, _BYTE, _BYTE, _BYTE);

	/*! Return the size of the color array */
	inline _DWORD Count() const;

	/*! Load a IRGB file (text file).
		IRGB file format is simple: all colors
		are stored in the array order
		in RGB (three byte per color).
		\param	File*	: file handler
		\param	_DWORD	: maximum number of colors to load
	*/
	void LoadRGB(File *, _DWORD = 256);

	/*! Load a IRGB file (text file).
		IRGB file format is simple: all colors
		are stored in the array order
		in RGB (three byte per color).
		\param	char*	: file name
		\param	_DWORD	: number of colors to load
	*/
	void LoadRGB(const char *, _DWORD = 256);

	/*! Load a color array (three bytes R, G, B per entry) from memory
		\param	char*	: array of bytes
		\param	_DWORD	: number of colors to load
	*/
	void MapRGB(_BYTE *, _DWORD = 256);

	const IPalette &operator = (const IPalette &);

private:
	_DWORD 		nbcol;
	IRGBColor 	*pal;
	// this palette is a clone yes/no
	_BOOL		clone;
};


inline const IRGBColor &IPalette::operator[](_DWORD i) const
{ return pal[i]; }

inline const IRGBColor &IPalette::Get(_DWORD i) const
{ return pal[i]; }

inline _DWORD IPalette::Count() const
{ return nbcol; }


}

#endif
