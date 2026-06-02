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

	(c) 2000-2001 Henri Michelon

$Id: GResourceVector.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/addons/GUI.hpp>
#include "GResourceVector.hpp"
using namespace NMO;
 
//----------------------------------------------
GResourceVector::GResourceVector(const Stringz&RES):
	GResource(RES),
	style(FLAT), width(0), height(0), flat(FALSE)
{
	SplitResString(RES);
}


//----------------------------------------------
void GResourceVector::SplitResString(const Stringz&RES)
{
	List<Stringz> res;
	RES.Split(res, ',');
	ListIterator<Stringz> list(res);
	if (res.Count() > 0) {
		width = _DWORD(list[1]);
	}
	if (res.Count() > 1) {
		height = _DWORD(list[2]);
	}
	if (res.Count() > 2) {
		if (list[3] == "RAISED") { style = RAISED; }
		if (list[3] == "LOWERED") { style = LOWERED; }
		if (list[3] == "FLAT") { style = FLAT; }
	}
	if (res.Count() > 3) {
		flat = (list[4] == "FLAT");
	}
}
