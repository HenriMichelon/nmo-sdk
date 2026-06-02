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

$Id: Winsock.cpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
------------------------------------------------------------------*/
#include <nmo/NMO.hpp>
#if defined(WIN32) || defined(_WIN32)
#include "Winsock.hpp"
using namespace NMO;

_DWORD Winsock::nInst = 0;

//------------------------------------
_BOOL Winsock::Init()
{
	if (!nInst++)
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		wVersionRequested = MAKEWORD(2, 0); 
		if (WSAStartup(wVersionRequested, &wsaData )) 
			return FALSE;
	}
	return TRUE;
}


//------------------------------------
void Winsock::Done()
{
	if (!--nInst) 
		WSACleanup();
}


//------------------------------------
const Stringz Winsock::LastError()
{
	switch (WSAGetLastError())
	{
	case WSAEINTR                : return "WSAEINTR";
	case WSAEBADF                : return "WSAEBADF";
	case WSAEACCES               : return "WSAEACCES";
	case WSAEFAULT               : return "WSAEFAULT";
	case WSAEINVAL               : return "WSAEINVAL";
	case WSAEMFILE               : return "WSAEFILE";

	/*
	 * Windows Sockets definitions of regular Berkeley error constants
	 */
	case WSAEWOULDBLOCK          : return "WSAEWOULDBLOCK";
	case WSAEINPROGRESS          : return "WSAEINPROGRESS";
	case WSAEALREADY             : return "WSAEALREADY";
	case WSAENOTSOCK             : return "WSAENOTSOCK";
	case WSAEDESTADDRREQ         : return "WSAEDESTADDRREQ";
	case WSAEMSGSIZE             : return "WSAESGSIZE";
	case WSAEPROTOTYPE           : return "WSAEPROTOTYPE";
	case WSAENOPROTOOPT          : return "WSAENOPROTOOPT";
	case WSAEPROTONOSUPPORT      : return "WSAEPROTONOSUPPORT";
	case WSAESOCKTNOSUPPORT      : return "WSAESOCKNOSUPPORT";
	case WSAEOPNOTSUPP           : return "WSAEOPNOTSUPP";
	case WSAEPFNOSUPPORT         : return "WSAEPFNOSUPPORT";
	case WSAEAFNOSUPPORT         : return "WSAEAFNOSUPPORT";
	case WSAEADDRINUSE           : return "WSAEADDRINUSE";
	case WSAEADDRNOTAVAIL        : return "WSAEADDRNOTAVAIL";
	case WSAENETDOWN             : return "WSAENETDOWN";
	case WSAENETUNREACH          : return "WSAENETUNREACH";
	case WSAENETRESET            : return "WSAENETRESET";
	case WSAECONNABORTED         : return "WSAECONNABORTED";
	case WSAECONNRESET           : return "WSAECONNRESET";
	case WSAENOBUFS              : return "WSAENOBUFS";
	case WSAEISCONN              : return "WSAEISCONN";
	case WSAENOTCONN             : return "WSAENOTCONN";
	case WSAESHUTDOWN            : return "WSAESHUTDOWN";
	case WSAETOOMANYREFS         : return "WSAETOOMANYREFS";
	case WSAETIMEDOUT            : return "WSAETIMEDOUT";
	case WSAECONNREFUSED         : return "WSAECONNREFUSED";
	case WSAELOOP                : return "WSAELOOP";
	case WSAENAMETOOLONG         : return "WSAENAMETOOLONG";
	case WSAEHOSTDOWN            : return "WSAEHOSTDOWN";
	case WSAEHOSTUNREACH         : return "WSAEHOSTUNREACH";
	case WSAENOTEMPTY            : return "WSAENOTEMPTY";
	case WSAEPROCLIM             : return "WSAEPROCLIM";
	case WSAEUSERS               : return "WSAEUSERS";
	case WSAEDQUOT               : return "WSAEDQUOT";
	case WSAESTALE               : return "WSAESTALE";
	case WSAEREMOTE              : return "WSAEREMOTE";

	/*
	 * Extended Windows Sockets error constant definitions
	 */
	case WSASYSNOTREADY          : return "WSAESYSNOTREADY";
	case WSAVERNOTSUPPORTED      : return "WSAEVERNOTSUPPORTED";
	case WSANOTINITIALISED       : return "WSAENOTINITIALISED";
	case WSAEDISCON              : return "WSAEDISCON";
	case WSAENOMORE              : return "WSAENOMORE";
	case WSAECANCELLED           : return "WSAECANCELLED";
	case WSAEINVALIDPROCTABLE    : return "WSAEINVALIDPROCTABLE";
	case WSAEINVALIDPROVIDER     : return "WSAEINVALIDPROVIDER";
	case WSAEPROVIDERFAILEDINIT  : return "WSAEPROVIDERFAILEDINIT";
	case WSASYSCALLFAILURE       : return "WSAESYSCALLFAILUER";
	case WSASERVICE_NOT_FOUND    : return "WSAESERVICE_NOT_FOUND";
	case WSATYPE_NOT_FOUND       : return "WSAETYPE_NOT_FOUND";
	case WSA_E_NO_MORE           : return "WSA_E_NO_MORE";
	case WSA_E_CANCELLED         : return "WSA_E_CANCELLED";
	case WSAEREFUSED             : return "WSAEREFUSED";
	default:
		return "";
	}
}
#endif
