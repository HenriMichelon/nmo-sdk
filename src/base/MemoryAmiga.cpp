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

	(c) 2000-2001 Henri Michelon 

$Id: MemoryAmiga.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <clib/exec_protos.h>

#ifdef WARPUP
	#include <powerpc/powerpc_protos.h>
	#define N_FREEVEC FreeVecPPC
	#define N_COPYMEM CopyMemPPC
#else
	#define N_FREEVEC FreeVec
	#define N_COPYMEM CopyMem
#endif

#include <nmo/Base.hpp>
using namespace NMO;


_PTR Memory::Alloc(_DWORD SIZE)
{
	ASSERTMSG(SIZE, "Memory::Alloc: allocation of a zero bytes block");
#ifdef WARPUP
	return (_PTR) AllocVecPPC(SIZE, MEMF_PUBLIC, 0);
#else
	return (_PTR) AllocVec(SIZE, MEMF_PUBLIC);
#endif
}


void Memory::Free(_PTR PTR)
{
	ASSERTMSG(PTR, "Memory::Free: Trying to free a null pointer");
	N_FREEVEC(PTR);
}



void Memory::Copy(_PTR A, const _PTR B, _DWORD C)
{ 
	ASSERTMSG(A, "Memory::Copy: copying to a NULL destination");
	ASSERTMSG(B, "Memory::Copy: copying from a NULL source");
	N_COPYMEM((_PTR)B, A, C); 
}


void Memory::Fill(_PTR A, _BYTE V, _DWORD S)
{ 
	ASSERTMSG(A, "Memory::Fill: filling a NULL block");
	_BYTE *p = (_BYTE*)A;
	_DWORD i;
	for (i=0; i<S; i++) {
		*(p++) = V;
	}
}

#endif
