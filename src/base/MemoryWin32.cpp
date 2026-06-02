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

$Id: MemoryWin32.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;

_PTR Memory::Alloc(_DWORD SIZE)
{
	ASSERTMSG(SIZE, "Memory::Alloc: allocation of a zero bytes block");
	return (_PTR) GlobalAlloc(GMEM_FIXED, SIZE);
}


void Memory::Free(_PTR PTR)
{
	ASSERTMSG(PTR, "Memory::Free: Trying to free a null pointer");
	GlobalFree((HGLOBAL)PTR);
}


#ifdef _MSC_VER // Microsoft Visual C++

void Memory::CopyWORD(_PTR A, const _PTR B, _DWORD C)
{ CopyMemory(A, B, C); }

void Memory::CopyDWORD(_PTR A, const _PTR B, _DWORD C)
{ CopyMemory(A, B, C); }

#endif // _MSC_VER



void Memory::Copy(_PTR A, const _PTR B, _DWORD C)
{ CopyMemory(A, B, C); }


void Memory::Fill(_PTR A, _BYTE V, _DWORD S)
{ FillMemory (A, S, V); }


#endif
