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
#ifndef _INTERFACE_DISPLAY_H_
#define _INTERFACE_DISPLAY_H_

namespace NMO {

/*!
\brief		2D Display device


			 simple & double buffered


\author		(c) 1998-2002 Henri Michelon 
\version	$Id: IDisplay.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------



class IDisplay : public NMOObject
{
public:
	/*! Display type. */
	typedef enum {
		/*! Normal display */
		DISPLAY_SIMPLE,
		/*! Double buffered */
		DISPLAY_DOUBLE
	} DisplayType;

	IDisplay(DisplayType);
	virtual ~IDisplay();

	/*!	Set the clipping region.
		\param _DWORD: left
		\param _DWORD: top
		\param _DWORD:width
		\param _DWORD: height
	*/
	virtual void SetClip(_LONG, _LONG, _DWORD, _DWORD) = 0;
	virtual void SetClip(const IRect&);

	/*!	Get the clipping region.
		\param _DWORD: left
		\param _DWORD: top
		\param _DWORD:width
		\param _DWORD: height
	*/
	virtual void GetClip(_LONG&, _LONG&, _DWORD&, _DWORD&) = 0;

	/*! Set the current pen color */
	virtual void SetPenColor(const IRGBColor&);

	/*! Set the current transparency color */
	virtual void SetTransColor(const IRGBColor&);

	/*! Return the current transparency color */
	virtual const IRGBColor& TransColor() const;

	/*! Get the current pen color */
	virtual const IRGBColor& PenColor() const;

	/*! Draw a single pixel using the pen color
		\param _LONG: x
		\param _LONG: y
	*/
	virtual void DrawPixel (_LONG, _LONG) = 0;

	/*! Read the value of a pixel
		\param _LONG: x
		\param _LONG: y
		\param IRBGColor : pixel color
	*/
	virtual void GetPixel (_LONG, _LONG, IRGBColor&) = 0;

	/*! Draw a line using the pen color.
		\param _LONG: x first point
		\param _LONG: y first point
		\param _LONG: x second point
		\param _LONG: y second point
	*/
	virtual void DrawLine (_LONG, _LONG, _LONG, _LONG) = 0;

	/*! Draw a circle using the pen color.
		\param _LONG	: x (center)
		\param _LONG	: y (center)
		\param _DWORD	: beam
		\param _BOOL	: fill with pen color
		\param _DWORD	: step (space) between each pixel (min 1)
	*/
	virtual void DrawCircle (_LONG, _LONG, _DWORD, _BOOL = TRUE, _DWORD = 1) = 0;

	/*! Draw an horizontal line using the pen color.
		\param _LONG: x (left point)
		\param _LONG: y (left point)
		\param _DWORD: width in pixels
	*/
	virtual void DrawHLine (_LONG, _LONG, _DWORD) = 0;

	/*! Draw a filled rectangle using the pen color
		\param _LONG: x
		\param _LONG: y
		\param _DWORD: widht
		\param _DWORD: height
	*/
	virtual void DrawRect(_LONG, _LONG, _DWORD, _DWORD) = 0;

	/*! Draw a filled rectangle using the pen color */
	void DrawRect(const IRect&);

	/*! Allocate & fill a memory area containing a portion of the display;
		\param _LONG: x position of the display portion to grab
		\param _LONG: y ""
		\param _DWORD: width of the portion
		\param _DWORD: height ""
		\return the new pixmap or NULL if error.You must delete this with delete[].
	*/
	virtual _BYTE * GetRect(_LONG&, _LONG&, _DWORD&, _DWORD&) = 0;

	/*! Allocate & fill a memory area containing a portion of the display;
		\param IRect: position & size of the display portion to grab
		\return the new pixmap or NULL if error. You must delete this with delete[].
	*/
	virtual _BYTE * GetRect(IRect&);

	/*! Draw a pixmap (must be remapped before).
		\param const void*: pixmap bits
		\param _LONG: x
		\param _LONG: y
		\param _DWORD: width of the pixmap
		\param _DWORD:	height of the pixmap
		\param _BOOL: transparent yes/no
		\param _BOOL: clip bitmap yes/no
	*/
	virtual void Draw(const void *, _LONG, _LONG, _DWORD, _DWORD, _BOOL, _BOOL = TRUE);

	/*! Draw a pixmap (must be remapped before, no clipping)
		\param const void*: pixmap bits
		\param _LONG: x
		\param _LONG: y
		\param _DWORD: width of the pixmap
		\param _DWORD:	height of the pixmap
	*/
	virtual void DrawNoTransNoClip(const void *, _LONG, _LONG, _DWORD, _DWORD) = 0;

	/*! Draw a pixmap (must be remapped, no clipping).
		\param const void*: bitmap bits
		\param const Rect&: position & size of the bitmap
	*/
	virtual void DrawNoTransNoClip(const void *, const IRect &);

	/*! Draw a pixmap (must be remapped, transparent, no clipping).
		\param const void*: pixmap bits
		\param _LONG: x
		\param _LONG: y
		\param _DWORD: width of the pixmap
		\param _DWORD:	height of the pixmap
	*/
	virtual void DrawTransNoClip(const void *, _LONG, _LONG, _DWORD, _DWORD) = 0;

	/*! Draw a pixmap (must be remapped, transparent, no clipping).
		\param const void*: pixmap bits
		\param const Rect&: position & size of the pixmap
	*/
	virtual void DrawTransNoClip(const void *, const IRect &);

	/*! Draw a pixmap (must be remapped, no clipping).
		\param const void*: pixmap bits
		\param _LONG: x
		\param _LONG: y
		\param _DWORD: width of the pixmap
		\param _DWORD:	height of the pixmap
	*/
	virtual void DrawNoTransClip(const void *, _LONG, _LONG, _DWORD, _DWORD) = 0;

	/*! Draw a pixmap (must be remapped, clipping)
		\param const void*: pixmap bits
		\param const Rect&: position & size of the pixmap
	*/
	virtual void DrawNoTransClip(const void *, const IRect &);

	/*! Draw a pixmap (must be remapped, transparent, clipping)
		\param const void*: pixmap bits
		\param _LONG: x
		\param _LONG: y
		\param _DWORD: width of the pixmap
		\param _DWORD:	height of the pixmap
	*/
	virtual void DrawTransClip(const void *, _LONG, _LONG, _DWORD, _DWORD) = 0;

	/*! Draw a pixmap (must be remapped, transparent, clipping)
		\param const void*: pixmap bits
		\param const Rect&: position & size of the pixmap
	*/
	virtual void DrawTransClip(const void *, const IRect &);

	/*! Clear the whole display with the current pen color */
	virtual void Clear() = 0;

	/*! Refresh the display area */
	virtual void Refresh() = 0;

	/*! Refresh a part of the display area */
	virtual void Refresh(const IRect&);

	/*! Get the height of the display */
	virtual _DWORD Height() const = 0;

	/*! Get the width of the display */
	virtual _DWORD Width() const = 0;

	/*! Get display type */
	DisplayType Type() const;
	
	/*! Return TRUE if the display if also double buffered by the Window Manager */
	virtual _BOOL NativeDoubleBuffer() const;

	virtual _BOOL CreateBuffers() = 0;
	virtual void DestroyBuffers() = 0;

protected:
	DisplayType	dt;

private:	
	IRGBColor	coltrans;
	IRGBColor	colpen;
};

inline void IDisplay :: Refresh(const IRect&) { Refresh(); }
inline IDisplay::DisplayType IDisplay::Type() const { return dt; }
inline void IDisplay::SetClip(const IRect&R) { SetClip(R.left, R.top, R.width, R.height); }
inline void IDisplay::DrawRect(const IRect &R) { DrawRect(R.left, R.top, R.width, R.height); }
inline _BYTE* IDisplay::GetRect(IRect &R) { return GetRect(R.left, R.top, R.width, R.height); }
inline const IRGBColor& IDisplay::TransColor() const { return coltrans; };
inline const IRGBColor& IDisplay::PenColor() const { return colpen; };
inline void IDisplay::SetPenColor(const IRGBColor&C) { colpen = C; };
inline void IDisplay::SetTransColor(const IRGBColor&C) { coltrans = C; };

}

#endif
