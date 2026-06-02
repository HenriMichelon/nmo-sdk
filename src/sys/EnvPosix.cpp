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

$Id: EnvPosix.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-----------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <stdlib.h>
using namespace NMO;


//------------------------------------------------------
void Env::Delete(const Stringz&N)
{
	Stringz str = N + '=';
	putenv((_CHAR*)str);
}


//------------------------------------------------------
void Env::Write(const Stringz&N, const Stringz&V)
{
	Stringz str = N + '=' + V;
	putenv((_CHAR*)str);
}
