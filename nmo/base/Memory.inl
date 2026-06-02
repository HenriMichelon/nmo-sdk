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

$Id: Memory.inl,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef __DEBUG
#include <nmo/Base.hpp>
using namespace NMO;


//-------------------------------------------------------
inline void *operator new(size_t SIZE)
{
	return Memory::Alloc(_DWORD(SIZE));
}


//-------------------------------------------------------
inline void operator delete(_PTR PTR)
{
	Memory::Free(PTR);
}

//-------------------------------------------------------
inline void * operator new[](size_t SIZE)
{
	return Memory::Alloc(_DWORD(SIZE));
}


//-------------------------------------------------------
inline void operator delete[](_PTR PTR)
{
	Memory::Free(PTR);
}

#endif
