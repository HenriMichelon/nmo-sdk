/*                                ---------
                                  [NMO-SDK]
                                  ---------

    The contents of this file are subject to the NMO SDK Public License
    Version 1.1 (the "License"); you may not use this file except in
    compliance with the License. You may obtain a copy of the License at
    http://nmo-sdk.sourceforge.net/licence.html

    Software distributed under the License is distributed on an "AS IS"
    basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
    License for the specific language governing rights and limitations under
    the License.

	Some defines for use with shared object for portability

	(c) 2000-2001 Henri Michelon

$Id: SharedObject.h,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $
--------------------------------------------------------------------------*/
#ifndef _NMO_SHAREDOBJ_H_
#define _NMO_SHAREDOBJ_H_

#include <nmo/base/Includes.h>
#include <nmo/base/Types.h>

#ifdef _WIN32
#define SOInitFunction BOOL APIENTRY DllMain(HANDLE _H,DWORD _D,LPVOID _L)
#define SOInitReturn return TRUE;
#define SOExport __declspec(dllexport)
#endif

#ifdef _POSIX
#define SOInitFunction void _init()
#define SOInitReturn 
#define SOExport
#endif

#ifdef macintosh
#define SOInitFunction
#define SOInitReturn 
#define SOExport
#endif

#endif
