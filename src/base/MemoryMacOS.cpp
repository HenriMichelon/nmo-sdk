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

	(c) 1998-2002 Henri Michelon

$Id: MemoryMacOS.cpp,v 1.2 2002/11/29 13:27:52 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_MACOS) || defined(macintosh)
#include <nmo/Base.hpp>
using namespace NMO;

_PTR Memory::Alloc(_DWORD SIZE)
{
	ASSERT(SIZE);
	_PTR res = NewPtr(SIZE);
	if (!res) {
		Size size; 
		MaxMem(&size);
		res = NewPtr(SIZE);
		/*if (!res)  {
			throw NMOException("No enougth memory"); 
		}*/
	}
	return res;
}


void Memory::Free(_PTR PTR)
{
	ASSERT(PTR);
	DisposePtr((Ptr)PTR);
}


void Memory::Copy(_PTR D, const _PTR S, _DWORD C)
{ 
#ifdef __MWERKS__ // Metrowerks Code Warrior
	memcpy(D, S, C);
#else
	BlockMove(S, D, C); 
	--
#endif
}


void Memory::Fill(_PTR A, _BYTE V, _DWORD S)
{
	if (V) {
		_BYTE *p = (_BYTE*)A;
		_DWORD i;
		for (i=0; i<S; i++) { p[i] = V; }
	}
	else {
		BlockZero(A, S);
	}
}
#endif