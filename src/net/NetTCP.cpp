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

$Id: NetTCP.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
--------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Net.hpp>
using namespace NMO;

#ifdef _WIN32
# include "Winsock.hpp"
#define SOCKLENT int

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
#if defined(_IRIX) || defined(_AMIGAOS) || defined(_BEOS) || defined(_DARWIN)
	#define SOCKLENT int
#else
	#ifdef _BSD
		#define SOCKLENT socklen_t
	#else
		#define SOCKLENT size_t
	#endif
#endif

#endif // _WIN32

#ifdef _POSIX
#include <string.h>
#endif

#include "NetIDIP.hpp"
#include "NetTCP.hpp"


//--------------------------------------------------------------------------
NetTCP::NetTCP(const NetIDIP*ID, FileCfg &): NetStream(), bound(FALSE)
{
	netID = (NetIDIP*)ID;
	if (netID->sd == INVALID_SOCKET) {
		netID->sd = socket(PF_INET, SOCK_STREAM, 0);
		if (netID->sd == INVALID_SOCKET) {
			throw NMOException(netID->LastError());
		}
	}
	Debug(dprintf("socket %d: created\n", netID->sd);)
}


//--------------------------------------------------------------------------
NetTCP::~NetTCP()
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
NetID* NetTCP::WaitConnect()
{
	VALIDATETHIS;
	if (!bound) {
		if (bind(netID->sd, (sockaddr*)&netID->addr, sizeof(sockaddr_in)) ==
			SOCKET_ERROR) {
			return NULL;
		}
#ifdef _BSD
		if (listen(netID->sd, -1) == SOCKET_ERROR)
#else
		if (listen(netID->sd, 256) == SOCKET_ERROR)
#endif
			return NULL;
		bound = TRUE;
	}

	SOCKLENT len = sizeof(sockaddr_in);
	NetIDIP *id = (NetIDIP*) NetID::Create(netID->Driver(), netID->cfg);

	Debug(dprintf("socket %d: WaitConnect\n", netID->sd);)

	if ((id->sd = accept(netID->sd, (sockaddr*)&id->addr, &len)) ==
		SOCKET_ERROR) {
		delete id;
		return NULL;
	}

	id->addr.sin_port = ntohs(id->addr.sin_port);
	hostent *host = gethostbyaddr((_CHAR*)&id->addr.sin_addr,
								  sizeof(id->addr.sin_addr), AF_INET);
	if ((!host) && (h_errno == TRY_AGAIN))
	{
		Timer timer;
		timer.Sleep(5000);
		host = gethostbyaddr((_CHAR*)&id->addr.sin_addr, 
							sizeof(id->addr.sin_addr),
							AF_INET);
	}
	if (host)
		id->name = host->h_name;
	else
		id->name = id->Address();
	return id; 
}



//--------------------------------------------------------------------------
_BOOL NetTCP::Connect(const NetID*ID)
{
	VALIDATETHIS;
	NetIDIP *id = (NetIDIP*) ID;
	Debug(dprintf("socket %d: connect\n", netID->sd);)
	if (connect(netID->sd, (sockaddr*)&id->addr, sizeof(sockaddr_in)) == 
		SOCKET_ERROR)
		return FALSE;
	return TRUE;
}



//--------------------------------------------------------------------------
_DWORD NetTCP::Send(const NetID*ID, const _PTR ADR, _DWORD LEN)
{
	VALIDATETHIS;
	ASSERT(ID);

	NetIDIP *id;
	// use accept() socket for a server
	// instead of own socket
	if (bound)
		id = (NetIDIP*)ID;
	else
		id = netID;

	_LONG res = send(id->sd, (const char*)ADR, LEN, 0);
	if (res == SOCKET_ERROR) return 0;
	return res;
}


//--------------------------------------------------------------------------
_DWORD NetTCP::Receive(const _PTR ADR, _DWORD LEN, NetID*&ID, _DWORD MS)
{
	VALIDATETHIS;
	ASSERT(ID);

	NetIDIP *id;
	// use accept() socket for a server
	// instead of own socket
	if (bound)
		id = (NetIDIP*)ID;
	else
		id = netID;

	// wait for a new packet
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(id->sd, &fds);
	if (MS)
	{
		// set timeout value
		timeval tv;
		tv.tv_sec = MS / 1000;
		tv.tv_usec = (MS-(tv.tv_sec)*1000)*1000;
		select(64, &fds, NULL, NULL, &tv);
	}
	else
		select(64, &fds, NULL, NULL, NULL);
	if (!FD_ISSET(id->sd, &fds)) return 0;

	// Fill block with special value for debug
	Debug(Memory::Fill((_PTR)ADR, 0xAA, LEN);)

	// wait for a fixed size packet	
	_LONG res = recv(id->sd, (char*)ADR, LEN, 0);
	if (res == SOCKET_ERROR) return 0;
	return _DWORD(res);
}

