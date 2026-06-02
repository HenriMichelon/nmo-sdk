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

*/
#ifndef _NMO_MEMORY_H_
#define _NMO_MEMORY_H_

namespace NMO {

/*!
\brief	Memory to memory copy & fill functions

		This class define friend function for memory block
		manipulation.
		This functions are intended to be used in place
		of ANSI memcpy, bzero and memset function, since
		some OS have specific memory manipulation functions
		that are more optimized than the ANSI ones.
		
\author		(c) 1998-2001 Henri Michelon 
\version	$Id: Memory.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $*/
//--------------------------------------------------------------------------

class Memory
{
public:
	/*! Copy a bloc of memory byte per byte
		\param _PTR		destination bloc
		\param _PTR		source bloc
		\param _LONG	size of the source bloc, in bytes
	*/	
	static void Copy(_PTR, const _PTR, _DWORD);

	/*! Copy a bloc of memory word per word
		\param _PTR		destination bloc
		\param _PTR		source bloc
		\param _LONG	size of the source bloc, in bytes
	*/	
	static void CopyWORD(_PTR, const _PTR, _DWORD);

	/*! Copy a bloc of memory dword per dword
		\param _PTR		destination bloc
		\param _PTR		source bloc
		\param _LONG	size of the source bloc, in bytes
	*/	
	static void CopyDWORD(_PTR, const _PTR, _DWORD);

	/*! Fill a memory bloc with a value
		\param _PTR		target bloc
		\param _BYTE	value
		\param _DWORD	size of the bloc in bytes
	*/	
	static void Fill(_PTR, _BYTE, _DWORD);
	
	/*! Allocate a block of memory.
		This function is REALLY prefered to the ANSI ones.
		It's the function used by our new operator.
		\param _DWORD	size of block in bytes.
		\result NULL on error
	*/
	static _PTR Alloc(_DWORD);
	
	/* Deallocate a block of memory.
		Use this function on blocks allocated with Alloc only.
		\param	_PTR	block to delete
	*/
	static void Free(_PTR);
};

}

#ifndef __DEBUG

#ifdef __TenDRA__ // Tendra C++ compiler
	void *operator new(size_t) throw (__bad_alloc);
	void *operator new[](size_t) throw (__bad_alloc);
	void operator delete (_PTR) throw () ;
	void operator delete [] (_PTR) throw () ;
#else
	void *operator new(size_t);
	void *operator new[](size_t);
	void operator delete(_PTR);
	void operator delete[](_PTR);
#endif // __TenDRA__

#endif


#include "nmo/base/Memory.inl"


#endif
