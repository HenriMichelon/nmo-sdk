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
#ifndef _GUI_LINE_H_
#define _GUI_LINE_H_

namespace NMO {

/*!
\brief		An horizontal or vertical line....

\author		(c) 1998-2002 Henri Michelon 
\version	$Id: GLine.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GLine: public GWidget
{
public:
	typedef enum
	{
		NONE,
		HORIZ,
		VERT
	} LineStyle;

	GLine(LineStyle = HORIZ);

	LineStyle Style() const;
	void SetStyle(LineStyle);

private:
	LineStyle style;
};

class GHLine: public GLine { public: GHLine(): GLine(HORIZ) {} };
class GVLine: public GLine { public: GVLine(): GLine(VERT) {} };

inline GLine::LineStyle GLine::Style() const { return style; };

}

#endif
