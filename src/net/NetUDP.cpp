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

$Id: NetUDP.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-----------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Net.hpp>
using namespace NMO;

#ifdef _WIN32
# include <winsock2.h>
#else // _WIN32

#ifdef _BEOS
# include <NetKit.h>
# define PF_INET AF_INET
#else
# include <arpa/inet.h>
# include <sys/time.h>
#endif // _BEOS
const int INVALID_SOCKET = -1;
const int SOCKET_ERROR = -1;

#endif // _WIN32

#include "NetIDIP.hpp"
#include "NetUDP.hpp"

// type of the last argument of recvfrom()
#ifdef _LINUX
#define SOCKLENT socklen_t
#else
#define SOCKLENT int
#endif

//--------------------------------------------------------------------------
NetUDP::NetUDP(const NetIDIP*ID, FileCfg &): NetMessage()
{
	netID = (NetIDIP*)ID;
	if (netID->sd == INVALID_SOCKET) {
		netID->sd = socket(PF_INET, SOCK_DGRAM, 0);
		if (netID->sd != INVALID_SOCKET) {
			if (bind(netID->sd, (sockaddr*)&netID->addr, sizeof(sockaddr_in)) !=
				SOCKET_ERROR) {
				return;
			}
		}
	}
	else {
		return;
	}
	throw NMOException(netID->LastError());
}


//--------------------------------------------------------------------------
NetUDP::~NetUDP()
{
	if (netID->sd != INVALID_SOCKET) {
#ifdef _WIN32
		closesocket(netID->sd);
#else
		close(netID->sd);
#endif
		netID->sd = INVALID_SOCKET;
	}
}


//--------------------------------------------------------------------------
_DWORD NetUDP::Send(const NetID*ID, const _PTR ADR, _DWORD LEN)
{
	VALIDATETHIS;
	ASSERT(ID);
	return 0;
}


//--------------------------------------------------------------------------
_DWORD NetUDP::Receive(const _PTR ADR, _DWORD LEN, NetID*&ID, _DWORD MS)
{
	VALIDATETHIS;
	return 0;
}


