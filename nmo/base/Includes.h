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

	Include files for the core NMO SDK

	(c) 1998-2002 Henri Michelon

$Id: Includes.h,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_INCLUDES_H_
#define _NMO_INCLUDES_H_

/*-----------------------------------------------------
	AmigaOS 3.1+
-----------------------------------------------------*/
#if (defined(amigaos) || defined(amiga) || defined(AMIGA)) && ! defined(_AMIGA)
	#ifndef _AMIGA
		#define _AMIGA 1
	#endif
#endif

#ifdef _AMIGA
	#include <exec/types.h>
	#include <exec/types.h>
	#include <exec/exec.h>
	#include <dos/dos.h>
	#include <intuition/intuition.h>
	#include <intuition/intuitionbase.h>
	#include <clib/exec_protos.h>
	#include <clib/dos_protos.h>
	#include <clib/intuition_protos.h>
	#include <clib/alib_protos.h>
	#include <clib/console_protos.h>
	#ifdef __GNUC__
		#include <sys/types.h>
	#endif
#endif /* AMIGA */


/*-----------------------------------------------------
	MacOS 8.6+ (CarbonLib 1.2+), MacOS X
-----------------------------------------------------*/
#if defined(macintosh) || defined(_MACOS)
	#ifndef _MACOS
		#define _MACOS 1
	#endif
	#if defined(__MWERKS__) && ! defined(_NMO_BUILD_PCH)
		#ifdef __DEBUG
			#include "nmopch-debug.mch"
		#else
			#include "nmopch-release.mch"
		#endif // __DEBUG
	#else
		#ifdef __MACH__ // MacOS X
			#include <Carbon/Carbon.h>
		#endif
	#endif // __MWERKS__
	#undef _POSIX
#endif // macintosh


/*-----------------------------------------------------
	FreeBSD 3.5+, NetBSD 1.5+, OpenBSD 2.9+
-----------------------------------------------------*/
#if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)
	#ifndef _BSD
		#define _BSD 1
	#endif
	#ifndef _POSIX
		#define _POSIX 1
	#endif
#endif

/*-----------------------------------------------------
	Darwin 1.2+
-----------------------------------------------------*/
#if defined(__MACH__) && defined(__APPLE__)
	#ifndef _DARWIN
		#define _DARWIN 1
	#endif
	#ifndef _BSD
		#define _BSD 1
	#endif
	#ifndef _POSIX
		#define _POSIX 1
	#endif
#endif


/*-----------------------------------------------------
	Solaris
-----------------------------------------------------*/
#ifdef sun
	#ifndef _SUNOS
		#define _SUNOS 1
	#endif
	#ifndef _POSIX
		#define _POSIX 1
	#endif
#endif


/*-----------------------------------------------------
	Linux 2.2+
-----------------------------------------------------*/
#ifdef linux
	#ifndef _LINUX
		#define _LINUX 1
	#endif
	#ifndef _POSIX
		#define _POSIX 1
	#endif
#endif


/*-----------------------------------------------------
	IRIX 6.5+
-----------------------------------------------------*/
#ifdef sgi
	#ifndef _IRIX
		#define _IRIX 1
	#endif
	#ifndef _POSIX
		#define _POSIX 1
	#endif
#endif


/*-----------------------------------------------------
	QNX RTP (6.0+)
-----------------------------------------------------*/
#ifdef __QNX__
	#ifndef _QNX
		#define _QNX 1
	#endif
	#ifndef _POSIX
		#define _POSIX 1
	#endif
#endif


/*-----------------------------------------------------
	AtheOS 0.x
-----------------------------------------------------*/
#ifdef __ATHEOS__
# ifndef _ATHEOS
#  define _ATHEOS 1
# endif
# ifndef _POSIX
#  define _POSIX 1
# endif
#endif
  

/*-----------------------------------------------------
	Microsoft.Net Beta 1	
-----------------------------------------------------*/
#ifdef _MANAGED
	#using <mscorlib.dll>
	using namespace System;
	using namespace System::Globalization;
	#define HANDLE void*
	#define NULL 0
	#include <string.h>
#endif


/*-----------------------------------------------------
	Windows 32 (4.x, NT 4.x, 5.x)
-----------------------------------------------------*/
#if defined(WIN32) && ! defined(_WIN32) && ! defined(_MANAGED)
	#define _WIN32 1
#endif

#if defined(_WIN32) && ! defined(_MANAGED)
	#if defined(_DEBUG) && ! defined(__DEBUG)
		#define __DEBUG 1
	#endif
	#define VC_EXTRALEAN
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#ifdef _MSC_VER
		#include <mmsystem.h>
		#include <process.h>
		#include <io.h>
		#include <signal.h>
		#include <fcntl.h>
		#include <sys/utime.h>
		#include <time.h>
		#include <sys/stat.h>
		#include <stdio.h>
		#include <stdlib.h>
		#include <direct.h>
		#include <time.h>
		#include <winreg.h>
		#include <tchar.h>
		#include <ddraw.h>
		#include <dsound.h>
		#pragma comment(lib, "winmm")
		#pragma comment(lib, "Wininet")
		#pragma comment(lib, "ddraw")
		#pragma comment(lib, "dsound")
		#pragma comment(lib, "dxguid")
		#pragma comment(lib, "Ws2_32")
	#endif
#endif /* _WIN32 */


/*-----------------------------------------------------
	BeOS R5+
----------------------------------------------------*/
#if defined(_BEOS) || defined(__BEOS__)
	#ifndef _BEOS
		#define _BEOS 1
	#endif
	#define _POSIX 1
	#include <AppKit.h>
	#include <InterfaceKit.h>
#endif /* _BEOS */


/*-----------------------------------------------------
	All POSIX compatibles systems
-----------------------------------------------------*/
#ifdef _POSIX
	#include <sys/types.h>
	#include <unistd.h>
#endif


/*-----------------------------------------------------
	Standards C ANSI includes
-----------------------------------------------------*/
#include <math.h>

#endif /* _NMO_INCLUDES_H_ */
