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
#ifndef _INTERFACE_FONT_H_
#define _INTERFACE_FONT_H_

namespace NMO {

/*!
\brief		Font display


\author		(c) 1998-2001 Henri Michelon
\version	$Id: IFont.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

#define IFONT_CACHEBUFFERSIZE 0xFFFF

class IFont: public NMOObject
{
public:
	/*! Load a font.
		Default font & size are architecture dependent
		\param String&	: font name, if "" then the engine default font is loaded
		\param String&	: engine name, if "" then the os/wm default engine is used
		\param _DWORD	: size, if 0 then a default size is selected
		\param _BOOL	: bold
		\param _BOOL	: italic
		\param _BOOL	: underline
		\param _BOOL	: use antialiasing
		\param ResourcesTable	: optional resources table for the data of this font
	*/
	static IFont *Create(const Stringz& = "", const Stringz& = "",
						_DWORD = 0, _BOOL = FALSE, _BOOL = FALSE, 
						_BOOL = FALSE, _BOOL = FALSE, ResourcesTable * = NULL);

	IFont(IFontEngine*);
	virtual ~IFont();

	void Bold(_BOOL);
	void Italic(_BOOL);
	void Underlined(_BOOL);

	/*! Select the size of the font
		\param _DWORD	: size (format is engine dependent)
	*/
	void SetSize(_DWORD);

	/*! Select the text (pen) color
		default color is (0, 0, 0)	
	*/
	void SetTextColor(const IRGBColor&);

	/*! Return the current RGB */
	IRGBColor BgColor();

	/*! Select the background color.
		default color is (255, 255, 255)
	*/
	void SetBgColor(const IRGBColor&);

	/*! Render a character into 8 bpp bitmap
		\return NULL if error
	*/
	IPixmap* Render(const UChar&);

	/*! Render a character into 8 bpp bitmap.
		Offsets are incremented
		\param char	: char to render
		\param _LONG	: xoffset
		\param _LONG	: yoffset
		\return NULL if error
	*/
	IPixmap* Render(const UChar&, _LONG&, _LONG&);

	/*! Return the maximum height (in pixels) for the font */
	_DWORD Height() const;
	/*! Return the width (in pixels) for a character */
	_DWORD Width(const UChar&);
	/*! Return the height (in pixels) for a character */
	_DWORD Height(const UChar&);
	/*! Return the width (in pixels) for a string */
	_DWORD Width(const UStringz&);
	/*! Return the height (in pixels) for a string */
	_DWORD Height(const UStringz&);

	/*! Draw a character into a display
		\param	IDisplay	: target display
		\param	char		: character to render
		\param	_LONG		: X position
		\param	_LONG		: Y position
		\param	_BOOL		: TRUE = draw a background rect
	*/
	void Draw(IDisplay&, const UChar&, _LONG, _LONG, _BOOL = FALSE);

	/*! Draw a character into a display.
		\param IDisplay	: target display
		\param char		: character to render
		\param _LONG		: X position
		\param _LONG		: Y position
		\param _BOOL		: TRUE = draw a background rect
	*/
	void Draw(IDisplay&, const UStringz&, _LONG, _LONG, _BOOL = FALSE);

private:
	IRGBColor	textColor;
	IRGBColor	bgColor;

	IFontEngine *engine;
	ICachedChar	cache[IFONT_CACHEBUFFERSIZE];

	ICachedChar &GetFont(const UChar&);
	void ResetCache();
};

inline _DWORD IFont::Height() const { return engine->Height(); }
inline IRGBColor IFont::BgColor() { return bgColor; }


}

#endif
