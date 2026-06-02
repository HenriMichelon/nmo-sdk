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

	(c) 1998-2001 Henri Michelon

$Id: Defines.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
using namespace NMO;

namespace NMO {

#if ! defined(_MACOS) || defined(__MACH__)

_DWORD SWAP_DWORD(_DWORD D)
{
	_DWORD	d;
	_BYTE	*pD = (_BYTE*) &D;
	_BYTE	*pd = (_BYTE*) &d;

	pd[3] = pD[0];
	pd[2] = pD[1];
	pd[1] = pD[2];
	pd[0] = pD[3];

	return d;

}

_LONG SWAP_LONG(_LONG D)
{
	_LONG d;
	_BYTE	*pD = (_BYTE*) &D;
	_BYTE	*pd = (_BYTE*) &d;

	pd[3] = pD[0];
	pd[2] = pD[1];
	pd[1] = pD[2];
	pd[0] = pD[3];

	return d;
}

_FLOAT SWAP_FLOAT(_FLOAT D)
{
	_FLOAT	d;
	_BYTE	*pD = (_BYTE*) &D;
	_BYTE	*pd = (_BYTE*) &d;

	pd[3] = pD[0];
	pd[2] = pD[1];
	pd[1] = pD[2];
	pd[0] = pD[3];

	return d;
}

#endif // _MACOS

}
