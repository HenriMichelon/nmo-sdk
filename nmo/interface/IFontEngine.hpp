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
#ifndef _INTERFACE_FONTENGINE_H_
#define _INTERFACE_FONTENGINE_H_

namespace NMO {

/*!
\brief		Font rendering engine.

			This class is normaly used only by the IFont class.
			But it can be used for special rendering purpose.
			Characters are rendered into a ICachedChar class
			for a face, position and attribute selection.


\author		(c) 1999-2001 Henri Michelon
\version	$Id: IFontEngine.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class IFontEngine: public NMOObject
{
public:
	IFontEngine();
	virtual ~IFontEngine();

	/*! Load a font.
		Default font & size are architecture dependent
		\param String&	: font name, if "" then a default font is loaded
		\param _DWORD	: size, if 0 then a default size is selected
		\param _BOOL	: bold
		\param _BOOL	: italic
		\param _BOOL	: underline
		\param _BOOL	: use antialiasing
	*/
	virtual _BOOL OpenFont(const Stringz&, _DWORD, _BOOL, _BOOL, _BOOL, _BOOL = FALSE);

	virtual _BOOL Bold(_BOOL);
	virtual _BOOL Italic(_BOOL);
	virtual _BOOL Underlined(_BOOL);
	virtual _BOOL SetSize(_DWORD);
	virtual _BOOL SetColors(const IRGBColor&, const IRGBColor&);

	/*! Return the maximum height for the font */
	_DWORD Height() const;

	/*! Return the maximum descent parameter 
		(number of pixels below the baseline) */
	_DWORD YMin() const;

	/*! Render a char. */
	virtual void FillCache(ICachedChar&, const UChar&);

	/*! Set the resources table to use for this font.*/
	void SetResources(ResourcesTable*);

protected:
	ResourcesTable	*restable;
	_BOOL			antialiasing;
	IPalette		*palette;
	_DWORD			height; // Maximum height of chars
	_DWORD			ymin; // font descent (number of lines under the font's baseline)

private:
#ifdef _AMIGA
	/* TODO: use an abstract pointer */
	static _DWORD	nInst; // For diskfont.library
	TextFont		*font;
#else
	_PTR			mIFontEngineAbstract;
#endif
};

inline void IFontEngine::SetResources(ResourcesTable*R) {  restable = R; }
inline _DWORD IFontEngine::Height() const { return height; }
inline _DWORD IFontEngine::YMin() const { return ymin; }

}

#endif
