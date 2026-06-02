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
#ifndef _NMO_GUI_TABS_H_
#define _NMO_GUI_TABS_H_

namespace NMO {

/*!
\brief		

\author		(c) 2001-2002 Henri Michelon 
\version	$Id: GTabs.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class GTabs: public GList
{
public:
	GTabs();
	virtual ~GTabs() {};

	virtual _DWORD Add(GWidget&, AlignmentType = LEFT, const Stringz& = "");
	virtual _DWORD Add(GWidget*, AlignmentType = LEFT, const Stringz& = "");
	virtual void Drop(_DWORD);

	void SetResources(const Stringz&);

private:
	GLine	leftline;
	GLine	rightline;
	Stringz	subresource;
	_DWORD	tabwidth;

	virtual void ComputeSums();
	virtual void EventSelectItem(_DWORD);
	virtual GWidget* EventMouseDown(IMouse::Button, _LONG, _LONG);
	virtual void EventResize(_BOOL);
};

}

#endif
