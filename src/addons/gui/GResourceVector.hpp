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

			(c) 2000 Henri Michelon 
			
$Id: GResourceVector.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------
#ifndef _NMO_GUI_RESOURCEVECTOR_H_
#define _NMO_GUI_RESOURCEVECTOR_H_

namespace NMO {

class GResourceVector: public GResource
{ 
public:
	typedef enum
	{
		FLAT,
		RAISED,
		LOWERED
	} Style;	

	Style		style;
	_DWORD		width;
	_DWORD		height;
	_BOOL		flat;
	
	GResourceVector(const Stringz&);
	virtual ~GResourceVector() {};

private:
	void SplitResString(const Stringz&);

};

}

#endif 
