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
#ifndef _GUI_TRACKBAR_H_
#define _GUI_TRACKBAR_H_

namespace NMO {

/*!
\brief		horizontal or vertical track bar

\author		(c) 1998-2002 Henri Michelon
\version	$Id: GTrackBar.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GTrackBar: public GValueSelect
{
public:
	GBox tracker;

	typedef enum
	{
		HORIZ,
		VERT
	} GTrackBarType;

	GTrackBar(GTrackBarType = HORIZ, _DWORD = 0, _DWORD = 0, _DWORD = 1, _DWORD = 0);
	virtual ~GTrackBar();
	
	GTrackBarType TrackBarType() const;

	void SetResources(const Stringz&);

private:
	GTrackBarType 	type;
	_BOOL			onScroll;
	_DWORD			scrollStart;
	IRect			mouseMoveRect;
	
	void OnTrackerMouseDown(GWidget&, GEvent*);
	virtual void EventResize(_BOOL);
	virtual void EventDraw(const IRect&, _BOOL = TRUE);
	virtual void EventMouseUp(IMouse::Button, _LONG, _LONG);
	virtual GWidget* EventMouseDown(IMouse::Button, _LONG, _LONG);
	virtual void EventMouseMove(IMouse::Button, _LONG, _LONG);

	virtual void EventRangeChange();
};

inline GTrackBar::GTrackBarType GTrackBar::TrackBarType() const { return type; };

class GVTrackBar: public GTrackBar
{
public:
	GVTrackBar(_DWORD I = 0, _DWORD A = 0, _DWORD V = 0, _DWORD S = 1): 
	  GTrackBar(VERT, I, A, V, S) {};
};


class GHTrackBar: public GTrackBar
{
public:
	GHTrackBar(_DWORD I = 0, _DWORD A = 0, _DWORD V = 0, _DWORD S = 1): 
	  GTrackBar(HORIZ, I, A, V, S) {};
};

}

#endif
