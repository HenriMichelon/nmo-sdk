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

	(c) 1999-2001 Henri Michelon

$Id: PipeBSD.cpp,v 1.1.1.1 2002/11/03 19:18:17 hmichelon Exp $
-----------------------------------------------------------------*/
#include "nmo/NMO.hpp"
using namespace NMO;
#include <unistd.h>

//-------------------------------------
Pipe::Pipe()
{
	pipe(_fds);
}

//-------------------------------------
Pipe::~Pipe()
{
	close(_fds[0]);
	close(_fds[1]);
}
