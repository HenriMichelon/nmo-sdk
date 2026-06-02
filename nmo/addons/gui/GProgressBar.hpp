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
#ifndef _GUI_PROGRESSBAR_H_
#define _GUI_PROGRESSBAR_H_

namespace NMO {

/*!
\brief		a rectangular progress bar

\author		(c) 1998-2002 Henri Michelon
\version	$Id: GProgressBar.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GProgressBar: public GValueSelect
{
public:
	typedef enum
	{
		HORIZ,
		VERT
	} ProgressBarType;
	
	typedef enum
	{
		NONE,
		PERCENT,
		VALUE
	} ProgressBarDisplay;

	GProgressBar(ProgressBarType = HORIZ, _DWORD = 0, _DWORD = 0, _DWORD = 1, _DWORD = 0);
	virtual ~GProgressBar() {};
	
	void SetType(ProgressBarType);
	ProgressBarType Type() const;

	void SetDisplayType(ProgressBarDisplay);
	ProgressBarDisplay DisplayType() const;

	void SetResources(const Stringz&);

private:
	ProgressBarDisplay	dtype;
	ProgressBarType		type;
	GBox				box;
};

inline GProgressBar::ProgressBarType GProgressBar::Type() const { return type; };
inline GProgressBar::ProgressBarDisplay GProgressBar::DisplayType() const { return dtype; };


class GVProgressBar: public GProgressBar
{
public:
	GVProgressBar(_DWORD I = 0, _DWORD A = 0, _DWORD V = 0): 
	  GProgressBar(VERT, I, A, V) {};
};


class GHProgressBar: public GProgressBar
{
public:
	GHProgressBar(_DWORD I = 0, _DWORD A = 0, _DWORD V = 0): 
	  GProgressBar(HORIZ, I, A, V) {};
};

}

#endif
