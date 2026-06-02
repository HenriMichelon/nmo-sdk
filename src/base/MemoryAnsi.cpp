/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://www.nmo-sdk.org/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	(c) 1998-2001 Henri Michelon 

$Id: MemoryAnsi.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/Base.hpp>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
using namespace NMO;


_PTR Memory::Alloc(_DWORD SIZE)
{
	return (_PTR) malloc(SIZE);
}



void Memory::Free(_PTR PTR)
{
	ASSERT(PTR);
	free((_PTR)PTR);
}


void Memory::Copy(_PTR A, const _PTR B, _DWORD C)
{ memcpy(A, B, C); }


void Memory::Fill(_PTR A, _BYTE V, _DWORD S)
{ memset(A, V, S); }
