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

	(c) 2000-2001 Henri Michelon

$Id: EnvAmiga.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-----------------------------------------------------------*/
#if defined(_AMIGA) || defined(amigaos)
#include <dos/var.h>
#include <clib/dos_protos.h>
#include <nmo/NMO.hpp>
using namespace NMO;

//------------------------------------------------------
void Env::Delete(const Stringz&N)
{
	DeleteVar((_CHAR*)N, 0);
}


//------------------------------------------------------
void Env::Write(const Stringz&N, const Stringz&V)
{
	SetVar((_CHAR*)N, (_CHAR*)V, -1, GVF_GLOBAL_ONLY);
}


//------------------------------------------------------
_BOOL Env::Read(const Stringz &S, Stringz&R)
{
	_CHAR buffer[1024];
	LONG len = GetVar((_CHAR*)S, buffer, 1024, 0);
	if (len != -1) { R = buffer; }
	return (len != -1);
}


//------------------------------------------------------
void Env::Environ(List<Stringz> &LIST)
{
}
#endif
