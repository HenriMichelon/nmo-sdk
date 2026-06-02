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

/*!
\brief	Definition of basic data types:

		+ numbers: 
			_BYTE		: 8 bits unsigned
			_WORD		: 16 bits unsigned
			_DWORD		: 32 bits unsigned
			_SHORT		: 8 bits signed
			_INT		: 16 bits signed
			_LONG		: 32 bits signed
			_LONGLONG	: 64 bits signed
			_FLOAT		: IEEE 32 bits floating point
			_DOUBLE		: IEEE 64 bits floating point

		+ misc:
			_PTR	: pointer
			_BOOL	: boolean
			_CHAR	: character
			_PID	: process id
			_SIG	: signal id

\author		(c) 1998-2002 Henri Michelon 

\version	$Id: Types.h,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------
#ifndef _NMO_TYPES_H_
#define _NMO_TYPES_H_


/*-----------------------------------------------------
 AmigaOS 3.x 
-----------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
typedef char	_CHAR;
typedef int		_PID;
//typedef !sig	_SIG
typedef UBYTE	_BYTE;	
typedef UWORD	_WORD;	
typedef ULONG	_DWORD;	
typedef BYTE	_SHORT;	
typedef WORD	_INT;	
typedef LONG	_LONG;	
typedef LONG	_LONGLONG;	
typedef float	_FLOAT;	
typedef double	_DOUBLE;
typedef long double	_LDOUBLE;
typedef BOOL	_BOOL;
#undef _PTR
#define _PTR	void*
#endif   /* _AMIGA */


/*-----------------------------------------------------
 All POSIX compliants systems
-----------------------------------------------------*/
#ifdef _POSIX
typedef pid_t			_PID;
typedef int				_SIG;
typedef char			_CHAR;
typedef unsigned char	_BYTE;
typedef unsigned short	_WORD;
typedef unsigned long	_DWORD;
typedef signed char		_SHORT;
#undef _INT
typedef signed short 	_INT;
typedef signed long		_LONG;
typedef float			_FLOAT;
typedef double			_DOUBLE;
typedef long double		_LDOUBLE;
typedef int				_BOOL;
#undef _LONGLONG
#define _LONGLONG		long long
#undef _PTR
#define _PTR			void*	/* stupid GCC */
#define _BOOL			_BOOL
#else /* _POSIX */


/*-----------------------------------------------------
 Windows 32 (4.x, NT 4.0)
-----------------------------------------------------*/
#ifdef _WIN32
typedef int			_SIG;
typedef char		_CHAR;
typedef char		_SHORT;
typedef short 		_INT;
typedef long		_LONG;
typedef __int64 	_LONGLONG;
typedef float		_FLOAT;
typedef double		_DOUBLE;
typedef long double	_LDOUBLE;
typedef void*		_PTR;
typedef unsigned char		_BYTE;
typedef unsigned short		_WORD;
typedef unsigned long		_DWORD;
typedef long				_BOOL;
typedef	long				_PID;
#endif /* _WIN32 */


/*-----------------------------------------------------
 MacOS 8+
-----------------------------------------------------*/
#ifdef _MACOS
typedef int				_PID;
typedef int				_SIG;
typedef char			_CHAR;
typedef UInt8			_BYTE;
typedef UInt16			_WORD;
typedef UInt32			_DWORD;
typedef SInt8			_SHORT;
typedef SInt16		 	_INT;
typedef SInt32			_LONG;
typedef SInt64			_LONGLONG;
typedef float			_FLOAT;
typedef double			_DOUBLE;
typedef long double		_LDOUBLE;
typedef Boolean			_BOOL;
typedef void*			_PTR;
#endif /* _MACOS */

#endif /* _POSIX */


#endif 
