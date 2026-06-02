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
#ifndef _GUI_PICTURE_H_
#define _GUI_PICTURE_H_

namespace NMO {
	
/*!
\brief		Widget to display a pixmap

\author		(c) 1998-2002 Henri Michelon 
\version	$Id: GPicture.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GPicture: public GWidget
{
public:
	GPicture(IPixmap* = NULL);
	virtual ~GPicture();

	// get automatic sizing flag
	_BOOL AutoSize() const;

	// set automatic sizing flag
	//	_BOOL	: TRUE = auto size when bitmap is changed
	void SetAutoSize(_BOOL);

	// get transparency flag for bitmaps
	_BOOL Transparency() const;

	// get transparency flag for bitmaps
	//	_BOOL	: TRUE = color RGB(0,0,0) is transparent
	void SetTransparency(_BOOL);

	// Get animation time
	// (number of 1/10 of seconds between two frames)
	//_DWORD AnimationTime() const;

	// Set animation time
	// (number of 1/10 of seconds between two frames)
	//	_DWORD	: time in 1/10 of seconds
	//void SetAnimationTime(_DWORD);

	// Get bitmap object
	IPixmap* Pixmap() const;

	// set bitmap object
	//	Bitmap*	: bitmap object, or NULL to
	//				disable bitmap display
	void SetPixmap(IPixmap*);

private:
	IPixmap	*pixmap;
	_BOOL	trans;
	_DWORD	animSpeed;
	_BOOL	autoSize;
	virtual void EventCreate();
};


inline _BOOL GPicture::Transparency() const { return trans; }
inline _BOOL GPicture::AutoSize() const { return autoSize; }

/*inline _DWORD GPicture::AnimationTime() const
{ return animSpeed; }

inline void GPicture::SetAnimationTime(_DWORD S)
{ animSpeed = S; }*/


}

#endif 
