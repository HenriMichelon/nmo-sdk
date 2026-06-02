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

$Id: Net.hpp,v 1.2 2002/11/29 14:26:05 hmichelon Exp $
--------------------------------------------------------------------------*/
#if !defined(_MACOS) && !defined(_AMIGA)
#define _BSDSOCKET
#endif

#include <nmo/net/NetID.hpp>
#include <nmo/net/NetProtocol.hpp>
#include <nmo/net/NetMessage.hpp>
#include <nmo/net/NetStream.hpp>
#include <nmo/net/InternetConnection.hpp>
#include <nmo/net/URL.hpp>
