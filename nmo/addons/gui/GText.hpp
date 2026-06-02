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
#ifndef _GUI_TEXT_H_
#define _GUI_TEXT_H_

namespace NMO {

/*!
\brief			A widget to display a line of text

\author			(c) 1998-2002 Henri Michelon 
\version		$Id: GText.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GText: public GPanel
{
public:
	GText(UStringz = "");

	const UStringz& Text() const;
	void SetText(const UStringz&);

	// get automatic sizing flag
	_BOOL AutoSize() const;

	// set automatic sizing flag
	//	_BOOL	: TRUE = auto size when text is changed
	void SetAutoSize(_BOOL);

	void ComputeSize();

protected:
	UStringz	text;

	virtual void EventCreate();

private:
	_BOOL	autoSize;
};


inline const UStringz& GText::Text() const { return (UStringz&)text; }
inline _BOOL GText::AutoSize() const { return autoSize; }

}

#endif // _GUI_TEXT_H_
