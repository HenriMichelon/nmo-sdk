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

$Id: Debug.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _DEBUG_H_
#define _DEBUG_H_

#undef dprintf
#undef Debug
#undef TRACE

#ifdef __DEBUG

	// FCTNAME return name of the current function
	#ifdef __GNUC__	// GNU C++ 2.7+
		#define FCTNAME __PRETTY_FUNCTION__
	#else
		#ifdef __STORM__	// H&P StormC
			#define FCTNAME __FUNC__
		#else
			#define FCTNAME ""
		#endif // __STORM__
	#endif // __GNUC__


	#define Debug(t) t
	#define DebugOutput(n) static const _CHAR* _nmodebugoutput = n

	void dopen(const _CHAR*);
	void dclose(void);
	void dprintf(const _CHAR *, ...);

	void _nmoassert(_BOOL, const _CHAR*, const _CHAR*, const _CHAR*, int);
	void _nmotrace(_CHAR*, int, _CHAR*);
	void _nmoinvalidThisPointer(_CHAR*, int, _CHAR*);

	#define TRACE _nmotrace((_CHAR*)__FILE__, (int)__LINE__, (_CHAR*)FCTNAME)
	#define ASSERT(t) { _nmoassert(!(t), "Assertion failed", NULL, (_CHAR*)__FILE__, (int)__LINE__); }
	#define ASSERTMSG(t, m)  { _nmoassert(!(t), "Assertion failed", (_CHAR*)m, (_CHAR*)__FILE__, (int)__LINE__); }
	#define POST(t, m) { _nmoassert(!(t), "Invalid post condition", (_CHAR*)m, (_CHAR*)__FILE__, (int)__LINE__); }
	#define PRE(t, m) { _nmoassert(!(t), "Invalid pre condition", (_CHAR*)m, (_CHAR*)__FILE__, (int)__LINE__); }
	#define INVARIANT(t, m) { _nmoassert(!(t), "Invalid invariant condition", (_CHAR*)m, (_CHAR*)__FILE__, (int)__LINE__); }
	#define VALIDATETHIS { if (!this) _nmoinvalidThisPointer((_CHAR*)__FILE__, (int)__LINE__, (_CHAR*)FCTNAME); }


	//----------------
	// memory operators

	extern _BOOL DebugNewTrace;

	_PTR operator new(size_t, const _CHAR *, _DWORD);
	_PTR operator new(size_t);
	_PTR operator new[](size_t);
	_PTR operator new[](size_t, const _CHAR*, _DWORD);
	void operator delete(_PTR);
	void operator delete[](_PTR);
	#ifdef _MSC_VER // Microsoft Visual C++
		void operator delete(_PTR, const _CHAR*, _DWORD);
		void operator delete[](_PTR, const _CHAR*, _DWORD);
	#endif // _MSC_VER

#else	// __DEBUG

	#define Debug(t)
	#define DebugOutput(n) 
	#define dprintf(t)
	#define TRACE
	#define ASSERT(t)
	#define ASSERTMSG(t, m)
	#define POST(t, m)
	#define PRE(t, m)
	#define INVARIANT(t, m)
	#define VALIDATETHIS

#endif // __DEBUG
#endif 
