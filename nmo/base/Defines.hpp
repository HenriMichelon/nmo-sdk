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

$Id: Defines.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_DEFINES_H_
#define _NMO_DEFINES_H_

namespace NMO {

#ifdef HAVE_CONFIGH
#include "../config.h"
#endif

#undef LC_ENG
#define LC_ENG

#if defined(_AMIGA) || defined(amigaos)
	static const char ALLFILES[]		= "#?";
	static const char ALLEXTS[]		= "#?";
	static const char PATHSEP			= '/';
	static const char PARENTDIR		= '/';
	static const char ROOTDIR			= ':';
	static const char CURRENTDIR[]	= "";
	static const char VARSEP			= ' ';
	static const char VARPRE			= '$';
	static const char VARPOS			= '\0';
	#define USEDRIVENAME	':'
	
	#if ! defined(AUTO_LIB)
		#define INTUITIONNAME	"intuition.library"
		#define INTUITIONVER	39L
		extern struct IntuitionBase *IntuitionBase;
	#endif

	#define GRAPHXNAME		"graphics.library"
	#define GRAPHXVERSION	0L
	extern struct GfxBase * GfxBase;
#endif /* _AMIGA */


#ifdef _MACOS
	static const char ALLFILES[]	= "*";
	static const char ALLEXTS[]		= "*";
	static const char PATHSEP		= ':';
	static const char ROOTDIR		= ':';
	static const char PARENTDIR[]	= "..";
	static const char VARSEP		= ':';
	static const char VARPRE		= '$';
	static const char VARPOS		= '\0';
	static const char CURRENTDIR[]	= "";
#endif // _MACOS


#ifdef WIN32
	#ifndef _WIN32
	#define _WIN32
	#endif
#endif

#ifdef _POSIX
	static const char ALLFILES[]	= "*";
	static const char ALLEXTS[]		= "*";
	static const char PATHSEP		= '/';
	static const char ROOTDIR		= '/';
	static const char PARENTDIR[]	= "..";
	static const char VARSEP		= ':';
	static const char VARPRE		= '$';
	static const char VARPOS		= '\0';
	static const char CURRENTDIR[]	= "./";
	#define FSCASESENSITIVE
	#define USERSECURITY
#else /* _POSIX */
	#ifdef _WIN32
		#define _X86
		#ifdef __BCPLUSPLUS__
			#define ALLFILES	"*.*"
			#define ALLEXTS		"*"
			#define PATHSEP		'\\'
			#define ROOTDIR		'\\'
			#define PARENTDIR	".."
			#define VARSEP		';'
			#define VARPRE		'$'
			#define VARPOS		'\0'
		#else
			static const char ALLFILES[]	= "*.*";
			static const char ALLEXTS[]		= "*";
			static const char PATHSEP		= '\\';
			static const char ROOTDIR		= '\\';
			static const char PARENTDIR[]	= "..";
			static const char VARSEP		= ';';
			static const char VARPRE		= '$';
			static const char VARPOS		= '\0';
		#endif
		#define USEDRIVENAME	':'
		#ifndef NULL
			#define NULL 0
		#endif
	#endif /* _WIN32 */
#endif /* _POSIX */

#if defined(__X86__) || defined(i386) || defined(__i386__) || defined(__INTEL__)
#define _X86 1
#endif

#if defined(sparc) || defined (__sparc__)
#define _SPARC 1
#endif

#ifdef mips
#define _MIPS 1
#endif

#if defined(mc68000) || defined(m68k)
#define _68K 1
#endif

#if defined(__POWERPC__) || defined(PPC) || defined(powerpc)
#define _PPC 1
#endif

#if defined(_68K) || defined(_PPC) || defined(_SPARC)
#define _BIGENDIAN 1
#endif

#if defined(_X86) || defined(_MIPS)
#define _LITTLEENDIAN 1
#endif


#undef TRUE
#undef FALSE
#undef ABS
#undef SIGN
#undef M_PI
#undef MIN
#undef MAX

static const _BYTE TRUE = 1;
static const _BYTE FALSE = 0;

inline _LONG MIN(_LONG N1, _LONG N2) { return (((N1) > (N2)) ? (N2) : (N1)); };
inline _DWORD MIN(_DWORD N1, _DWORD N2) { return (((N1) > (N2)) ? (N2) : (N1)); };
inline _FLOAT MIN(_FLOAT N1, _FLOAT N2) { return (((N1) > (N2)) ? (N2) : (N1)); };
inline _DOUBLE MIN(_DOUBLE N1, _DOUBLE N2) { return (((N1) > (N2)) ? (N2) : (N1)); };

inline _LONG MAX(_LONG N1, _LONG N2) { return (((N1) < (N2)) ? (N2) : (N1)); };
inline _DWORD MAX(_DWORD N1, _DWORD N2) { return (((N1) < (N2)) ? (N2) : (N1)); };
inline _FLOAT MAX(_FLOAT N1, _FLOAT N2) { return (((N1) < (N2)) ? (N2) : (N1)); };
inline _DOUBLE MAX(_DOUBLE N1, _DOUBLE N2) { return (((N1) < (N2)) ? (N2) : (N1)); };

inline _LONG ABS(_LONG N) { return ((N >= (0)) ? N : -N); };
inline _DOUBLE ABS(_DOUBLE N) { return ((N >= (0)) ? N : -N); };
inline _SHORT SIGN(_LONG a) { return (((a)<0) ? _SHORT(-1) : _SHORT(1)); };
inline _SHORT SIGN(_DOUBLE a) { return (((a)<0) ? _SHORT(-1) : _SHORT(1)); };

#ifdef __BCPLUSPLUS__
	#define M_PI 3.14159265358979323846
#else
	static const _DOUBLE M_PI = 3.14159265358979323846;
#endif

const _LONGLONG _BYTE_MAX	= 255;
const _LONGLONG _SHORT_MAX	= 127;
const _LONGLONG _SHORT_MIN	= -128;
const _LONGLONG _WORD_MAX	= 65535;
const _LONGLONG _INT_MAX	= 32767;
const _LONGLONG _INT_MIN	= -32768;
#if defined(__GNUC__)
	#if defined(mc68000)
		const _LONGLONG _DWORD_MAX	= 0xffffffff;
	#else
		const _LONGLONG _DWORD_MAX	= 0xffffffffffffffffULL;
	#endif
#else
	const _LONGLONG _DWORD_MAX	= 0xffffffffffffffff;
#endif
const _LONGLONG _LONG_MAX	= 2147483647;
const _LONGLONG _LONG_MIN	= -2147483647 - 1;
const _LDOUBLE _FLOAT_MAX	= 3.402823466e+38F;
const _LDOUBLE _FLOAT_MIN	= 1.175494351e-38F;
const _LDOUBLE _DOUBLE_MAX	= 1.7976931348623158e+308;
const _LDOUBLE _DOUBLE_MIN	= 2.2250738585072014e-308;


/* Big/Little endian problem */
#if defined(_MACOS) && defined(_PPC) && ! defined(__GNUC__)
	#define SWAP_WORD(W) ((_WORD)__lhbrx(&(W), 0))
	#define SWAP_INT(W) ((_INT)__lhbrx(&(W), 0))
	#define SWAP_DWORD(D) ((_DWORD)__lwbrx(&(D), 0))
	#define SWAP_LONG(D) ((_LONG)__lwbrx(&(D), 0))
	#define SWAP_FLOAT(D) ((_FLOAT)__lwbrx(&(D), 0))
#else
	#define SWAP_WORD(W) ((_WORD)((((W) << 8) & 0xFF00) | (((W) >> 8) & 0x00FF)))
	#define SWAP_INT(I) ((_INT)((((I) << 8) & 0xFF00) | (((I) >> 8) & 0x00FF)))
	_DWORD SWAP_DWORD(_DWORD D);
	_LONG SWAP_LONG(_LONG D);
	_FLOAT SWAP_FLOAT(_FLOAT D);
#endif

#ifdef _LITTLEENDIAN
	/* Big endian / host conversions */
	#define WORD2BE		SWAP_WORD
	#define DWORD2BE	SWAP_DWORD
	#define INT2BE		SWAP_INT
	#define	LONG2BE		SWAP_LONG
	#define	FLOAT2BE	SWAP_FLOAT
	#define BE2WORD		SWAP_WORD
	#define BE2DWORD	SWAP_DWORD
	#define BE2INT		SWAP_INT
	#define	BE2LONG		SWAP_LONG
	#define	BE2FLOAT	SWAP_FLOAT
	/* Little endian / host conversions */
	#define WORD2LE(x)		(x)
	#define DWORD2LE(x)		(x)
	#define INT2LE(x)		(x)
	#define	LONG2LE(x)		(x)
	#define FLOAT2LE(x)		(x)
	#define LE2WORD(x)		(x)
	#define LE2DWORD(x)		(x)
	#define LE2INT(x)		(x)
	#define	LE2LONG(x)		(x)
	#define LE2FLOAT(x)		(x)
#endif /* BIGENDIAN */

#ifdef _BIGENDIAN
	/* Big endian / host conversions */
	#define WORD2BE(x)		(x)
	#define DWORD2BE(x)		(x)
	#define INT2BE(x)		(x)
	#define	LONG2BE(x)		(x)
	#define FLOAT2BE(x)		(x)
	#define BE2WORD(x)		(x)
	#define BE2DWORD(x)		(x)
	#define BE2INT(x)		(x)
	#define	BE2LONG(x)		(x)
	#define BE2FLOAT(x)		(x)
	/* Little endian / host conversions */
	#define WORD2LE		SWAP_WORD
	#define DWORD2LE	SWAP_DWORD
	#define INT2LE		SWAP_INT
	#define	LONG2LE		SWAP_LONG
	#define	FLOAT2LE	SWAP_FLOAT
	#define LE2WORD		SWAP_WORD
	#define LE2DWORD	SWAP_DWORD
	#define LE2INT		SWAP_INT
	#define	LE2LONG		SWAP_LONG
	#define	LE2FLOAT	SWAP_FLOAT
#endif /* LITTLEENDIAN */


/* Some hacks for MS Managed C++ */
/*#ifdef _MANAGED
	#define ABSTRACT(x) ::System::Object* x
	#define CLASS public __gc class
#else
	#define ABSTRACT(x) _PTR x
	#define CLASS class
#endif */ /* _MANAGED */


/* new operator new for debug */
#ifdef __DEBUG
	#ifndef _USRDLL
		#ifndef _DEBUG_NO_DEBUG_NEW_
			#define DEBUG_NEW new(__FILE__, __LINE__)
			#define new DEBUG_NEW
	#endif
	#endif
#endif

}

#endif /* _NMO_DEFINES_H_ */
