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

$Id: NetIDIP.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-------------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#include <nmo/Net.hpp>
using namespace NMO;

#ifdef _WIN32
	#include "Winsock.hpp"
	#include <stdlib.h>
	const MAXHOSTNAMELEN = 1024;
#else
	const int INVALID_SOCKET = -1;
	#ifndef _BEOS
		#include <sys/param.h>
		#include <arpa/inet.h>
	#endif
#endif

#ifdef _POSIX
	#include <errno.h>
	#ifndef HAVE_SYSERRLIST
	extern const char * const sys_errlist[];
	#endif
#endif

#include "NetIDIP.hpp"
#include "NetTCP.hpp"
#include "NetUDP.hpp"


//---------------------------------------------------------------------------
NetIDIP::NetIDIP(): NetID()
{
#ifdef _WIN32
	Winsock::Init();
#endif
	sd = INVALID_SOCKET;
}


//---------------------------------------------------------------------------
NetIDIP::NetIDIP(FileCfg&CFG): NetID()
{
	Stringz forced;
	hostent	*host = NULL;

#ifdef _WIN32
	Winsock::Init();
#endif
	sd = INVALID_SOCKET;

	_WORD port = (_WORD)(_DWORD)CFG.Read("NET", "port", "0");
	if (!port)
		throw NMOException("IP: no ""NET*port"" value in config file\n");

	forced = CFG.Read("NET", "address");
	forced.LCase();
	if (!forced.Len()) {
		forced = CFG.Read("NET", "hostname");
	}
	if (forced.Len() && (forced != "any")) {
		host = gethostbyname((_CHAR*)forced);
	}
	if (!host) {
		char hostname[MAXHOSTNAMELEN];
		if (!gethostname(hostname, MAXHOSTNAMELEN))
			host = gethostbyname(hostname);
	}
	if (host && (host->h_addrtype == AF_INET)) {
		Memory::Fill(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		if (forced == "any") {
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else {
			Memory::Copy((_PTR)&addr.sin_addr, host->h_addr_list[0], sizeof(addr.sin_addr));
		}
		name = host->h_name;
	}
	else {
		throw NMOException("");
	}
}


//---------------------------------------------------------------------------
NetIDIP::~NetIDIP()
{
    if (sd != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(sd);
#else
        close(sd);
#endif
	}

#ifdef _WIN32
	Winsock::Done();
#endif
}


//---------------------------------------------------------------------------
Stringz NetIDIP::Name() const
{
	VALIDATETHIS;
	return name;
}


//---------------------------------------------------------------------------
Stringz NetIDIP::Address() const
{
	VALIDATETHIS;
	return inet_ntoa(addr.sin_addr);
}


//---------------------------------------------------------------------------
Stringz NetIDIP::LastError() const
{
	VALIDATETHIS;
#ifdef _WIN32
	return Winsock::LastError();
#else
#if !defined(_BEOS) && !defined(_AMIGA)
	return sys_errlist[errno];
#endif
#endif
}
