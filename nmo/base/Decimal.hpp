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

	fixed 256 bits numeric value (signed)
				
XXX: This is only experimental and not yet really written !

	(c) 2000-2001 Henri Michelon

$Id: Decimal.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_DECIMAL_H_
#define _NMO_DECIMAL_H_

namespace NMO {

const _BYTE PRECISION = 32;

class Decimal: public NMOObject
{
public:
	Decimal();
	Decimal(_LONG);
	Decimal(_INT);
	Decimal(_SHORT);
	Decimal(_DWORD);
	Decimal(_WORD);
	Decimal(_BYTE);
	Decimal(const Decimal&);
	
	operator _LONG() const;

	/*friend _BOOL operator > (const Decimal&, const Decimal&);
	friend _BOOL operator >= (const Decimal&, const Decimal&);
	friend _BOOL operator < (const Decimal&, const Decimal&);
	friend _BOOL operator <= (const Decimal&, const Decimal&);
	friend _BOOL operator == (const Decimal&, const Decimal&);
	friend _BOOL operator != (const Decimal&, const Decimal&);*/

private:
	_BYTE bytes[PRECISION];
	_BOOL sign;
};

}

#endif
