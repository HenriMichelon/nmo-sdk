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
#ifndef _NMO_INTERFACE_PIXMAP_H_
#define _NMO_INTERFACE_PIXMAP_H_

namespace NMO {

/*!
\brief		multi frames 2D DIB pixmap

			This class a 2D DIB pixmap that can
			handle multiple frames with time count
			between each frames.

			Currently the frames supports only 256 colors
			per pixel before being mapped to
			screen current color depth.
	
\author		(c) 1998-2001 Henri Michelon 
\version	$Id: IPixmap.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class IPixmap: public NMOObject
{
public:
	/*!
	\brief A frame in a IPixmap
	*/
	class IFrame: public NMOObject
	{
	public:
		/*! picture bits */
		_BYTE		*pic;
		/*! picture height (in pixel) */
		_DWORD		height;
		/*! picture width (in pixel) */
		_DWORD		width;
		/*! bytes per pixel */
		_BYTE		bpp;
		/*! total number of drawing steps to wait 
			before drawing the next frame */
		_WORD		sleepTimeTotal;
		/*! remaining number of steps for the
			current frame (==0 for the others frames) */
		_WORD		sleepTimeRemain;
		/*! RGB palette */
		IPalette	*pal;
		/*! TRUE is transparency supported */
		_BOOL		trans;
		/*! Transparent color of the frame */
		IRGBColor	transcolor;
		/*! next frame in the pixmap */
		IFrame		*next;
	};	

	IPixmap();
	virtual ~IPixmap();
	
	/*! Create a new pixmap (without copying images datas) */
	IPixmap* Clone() const;

	/*! copy pixmap (with copying the images datas) */
	IPixmap* Copy() const;

	/*!	map frames to the screen color depth */			
	void Remap();

	/*!	create a new frame and add it to the IPixmap
		\param _WORD = total number of drawing steps to wait  */
	IFrame *AddFrame(_WORD = 0);

	/*!	get a frame
		\param _WORD = number of the frame (0 based) */
	IFrame *Frame(_WORD) const;

	/*!	get number of frame */
	_WORD FramesCount() const;

	/*!	get height of the current frame */
	_DWORD Height() const;

	/*!	get width of the current frame */
	_DWORD Width() const;

	/*!	Draw current frame on a display
		Without transparency & clipping
		\param _LONG = X offset (0 based, left to right)
		\param _LONG = Y offset (0 based, top to bottom)
	*/
	void DrawNoTransNoClip(IDisplay&, _LONG, _LONG);
	
	/*!	Draw current frame on a display
		Without transparency, with clipping
		\param _LONG = X offset (0 based, left to right)
		\param _LONG = Y offset (0 based, top to bottom)
	*/
	void DrawNoTransClip(IDisplay&, _LONG, _LONG);
	
	/*!	Draw current frame on a display
		with transparency & without clipping
		\param _LONG = X offset (0 based, left to right)
		\param _LONG = Y offset (0 based, top to bottom)
	*/
	void DrawTransNoClip(IDisplay&, _LONG, _LONG);
	
	/*!	Draw current frame on a display
		With transparency & clipping
		\param _LONG = X offset (0 based, left to right)
		\param _LONG = Y offset (0 based, top to bottom)
	*/
	void DrawTransClip(IDisplay&, _LONG, _LONG);

	/*!	Draw current frame on a display
		\param _LONG = X offset (0 based, left to right)
		\param _LONG = Y offset (0 based, top to bottom)
		\param _BOOL = use transparent color RGB(0, 0, 0) yes/no
		\param _BOOL = do clipping yes/no
	*/
	void Draw(IDisplay&, _LONG, _LONG, _BOOL = TRUE, _BOOL = TRUE);

	/*! Blit a IPixmap on current the frame
		\param _LONG = X offset (0 based, left to right)
		\param _LONG = Y offset (0 based, top to bottom)
	*/
	void Draw(IPixmap*, _DWORD, _DWORD);

	/*! Create a new IPixmap from a rectangle of the current frame
		\param _DWORD	: x offset
		\param _DWORD	: y offset
		\param _DWORD	: width of the rectangular part
		\param _DWORD	: height of the rectangular part
	*/
	IPixmap *GetRect(_DWORD, _DWORD, _DWORD, _DWORD);

	/*! Create a new frame from a rectangle of a display
		\param IDisplay: display to grab
		\param _LONG	: x offset
		\param _LONG	: y offset
		\param _DWORD	: width of the rectangular part
		\param _DWORD	: height of the rectangular part
		\param values can be clipped
	*/
	void Grab(IDisplay&, _LONG&, _LONG&, _DWORD&, _DWORD&);

	/*! Create a new frame from a rectangle of a display
		\param IDisplay: display to grab
		\param _LONG	: x offset
		\param _LONG	: y offset
		\param _DWORD	: width of the rectangular part
		\param _DWORD	: height of the rectangular part
		\param values can be clipped
	*/
	void Grab(IDisplay&, IRect&);

	/*! Write a pixel in the current frame
		\param	_DWORD	: X
		\param	_DWORD	: Y
		\param	IRGBColor: color of the pixel
	*/
	void PutPixel(_DWORD, _DWORD, const IRGBColor&);

	/*! Set transparent color for all frames */
	void SetTransColor(const IRGBColor&);

	/*! Get the transparent color of the current frame */
	const IRGBColor& TransColor() const;

private:
	// head of list of frames
	IFrame		*frames;
	// total number of frames
	_WORD			count;
	// this IPixmap is a clone yes/no
	_BOOL			clone;
	// this IPixmap is already remaped
	_BOOL			remaped;

};


inline void IPixmap :: Grab(IDisplay&D, IRect&R)
{ Grab(D, (_LONG&)R.left, (_LONG&)R.top, R.width, R.height); }

}

#endif
