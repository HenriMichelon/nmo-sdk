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

      IP network identifier

      (c) 1999-2001 Henri Michelon

$Id: NetIDIP.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
-----------------------------------------------------------*/
#ifndef _NMO_NET_ID_BSDSOCK_H_
#define _NMO_NET_ID_BSDSOCK_H_

#ifdef _WIN32
# include "Winsock.hpp"
#else
extern "C" {
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
}
#endif

namespace NMO {

const _CHAR TCPDRIVERNAME[] = "TCP";
const _CHAR UDPDRIVERNAME[] = "UDP";

class NetIDIP: public NetID
{
public:
	// FileCfg parameters:
	//	NET*port : port to use
	//	raise NMOException on error
	NetIDIP(FileCfg&);
	NetIDIP();
	virtual ~NetIDIP();

	virtual Stringz Name() const;
	virtual Stringz Address() const;
	virtual Stringz LastError() const;

private:
	int			sd;
	Stringz		name;
	sockaddr_in	addr;

	friend class NetTCP;
	friend class NetUDP;
};

}

#endif
