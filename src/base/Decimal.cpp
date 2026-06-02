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

$Id: Decimal.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;


//-----------------------------------------------------------
Decimal::Decimal()
{
	Memory::Fill((_PTR)bytes, 0, PRECISION*sizeof(_BYTE));
	sign = FALSE;
}


//-----------------------------------------------------------
Decimal::Decimal(_BYTE V)
{
	Memory::Fill((_PTR)bytes, 0, PRECISION*sizeof(_BYTE));
	bytes[0] = V;
	sign = FALSE;
}


//-----------------------------------------------------------
Decimal::Decimal(_WORD V)
{
	Memory::Fill((_PTR)bytes, 0, PRECISION*sizeof(_BYTE));
	_WORD v = WORD2BE(V);
	bytes[0] = _BYTE(v & 0xFF);
	bytes[1] = _BYTE((v >> (1*8)) & 0xFF);
	sign = FALSE;
}


//-----------------------------------------------------------
Decimal::Decimal(_DWORD V)
{
	Memory::Fill((_PTR)bytes, 0, PRECISION*sizeof(_BYTE));
	_DWORD v = DWORD2BE(V);
	bytes[0] = _BYTE(v & 0xFF);
	bytes[1] = _BYTE((v >> (1*8)) & 0xFF);
	bytes[2] = _BYTE((v >> (2*8)) & 0xFF);
	bytes[3] = _BYTE((v >> (3*8)) & 0xFF);
	sign = FALSE;
}


//-----------------------------------------------------------
Decimal::Decimal(_SHORT V)
{
	Memory::Fill((_PTR)bytes, 0, PRECISION*sizeof(_BYTE));
	bytes[0] = _BYTE(V & 0xEF);
	sign = (V & 0x80) != 0;
}


//-----------------------------------------------------------
Decimal::Decimal(_INT V)
{
	Memory::Fill((_PTR)bytes, 0, PRECISION*sizeof(_BYTE));
	_INT v = INT2BE(V);
	bytes[0] = _BYTE(v & 0xFF);
	bytes[1] = _BYTE((v >> (1*8))& 0xEF);
	sign = (v & 0x8000) != 0;
}


//-----------------------------------------------------------
Decimal::Decimal(_LONG V)
{
	Memory::Fill((_PTR)bytes, 0, PRECISION*sizeof(_BYTE));
	_LONG v = LONG2BE(V);
	bytes[0] = _BYTE(v & 0xFF);
	bytes[1] = _BYTE((v >> (1*8))& 0xFF);
	bytes[2] = _BYTE((v >> (2*8))& 0xFF);
	bytes[3] = _BYTE((v >> (3*8))& 0xEF);
	sign = (v & 0x80000000) != 0;
}


//-----------------------------------------------------------
Decimal::Decimal(const Decimal&D)
{
	Memory::Copy((_PTR)bytes, (_PTR)D.bytes, PRECISION*sizeof(_BYTE));
}



//-----------------------------------------------------------
Decimal::operator _LONG() const
{
	_LONG l = bytes[0] + 
				(bytes[1] << (1*8)) +
				(bytes[2] << (2*8)) +
				((bytes[3] & 0xEF) << (3*8));
	if (sign)
		l |= 0x80000000;
	return BE2LONG(l);
}


/*//-----------------------------------------------------------
_BOOL operator > (const Decimal&D1, const Decimal&D2)
{
	if (D1.sign && (!D2.sign)) return FALSE;
	if (D1 == D2) return FALSE;
	for (_BYTE i=(PRECISION-1); i<=0; i--)
		if (D2.bytes[i] > D1.bytes[i]) return FALSE;
	return TRUE;
}


//-----------------------------------------------------------
_BOOL operator >= (const Decimal&D1, const Decimal&D2)
{
	if (D1.sign && (!D2.sign)) return FALSE;
	if (D1 == D2) return TRUE;
	for (_BYTE i=(PRECISION-1); i<=0; i--)
		if (D2.bytes[i] > D1.bytes[i]) return FALSE;
	return TRUE;
}


//-----------------------------------------------------------
_BOOL operator < (const Decimal&D1, const Decimal&D2)
{
	if (D2.sign && (!D1.sign)) return FALSE;
	if (D1 == D2) return FALSE;
	for (_BYTE i=(PRECISION-1); i<=0; i--)
		if (D1.bytes[i] > D2.bytes[i]) return FALSE;
	return TRUE;
}


//-----------------------------------------------------------
_BOOL operator <= (const Decimal&D1, const Decimal&D2)
{
	if (D2.sign && (!D1.sign)) return FALSE;
	if (D1 == D2) return TRUE;
	for (_BYTE i=(PRECISION-1); i<=0; i--)
		if (D1.bytes[i] > D2.bytes[i]) return FALSE;
	return TRUE;
}


//-----------------------------------------------------------
_BOOL operator == (const Decimal&D1, const Decimal&D2)
{
	if (D1.sign != D2.sign) return FALSE;
	for (_BYTE i=0; i<PRECISION; i++)
		if (D1.bytes[i] != D2.bytes[i]) return FALSE;
	return TRUE;
}


//-----------------------------------------------------------
_BOOL operator != (const Decimal&D1, const Decimal&D2)
{
	return (!(D1 == D2));
}


*/
