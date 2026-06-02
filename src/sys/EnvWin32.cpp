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

	(c) 1999-2001 Henri Michelon

$Id: EnvWin32.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
---------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
using namespace NMO;
#ifndef _MSC_VER
# include <stdlib.h>
#endif

//------------------------------------------------------
void Env::Write(const Stringz &S, const Stringz&R)
{
	Stringz v = S + '=' + R;
#ifdef _MSC_VER
	_putenv((_CHAR*)v);
#else
	putenv((_CHAR*)v);
#endif
	//SetEnvironmentVariable((_CHAR*)S, (_CHAR*)R);
}


//------------------------------------------------------
void Env::Delete(const Stringz &S)
{
	SetEnvironmentVariable((_CHAR*)S, NULL);
}
#endif
