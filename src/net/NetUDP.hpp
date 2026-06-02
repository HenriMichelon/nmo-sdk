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

						(c) 1999-2000 Henri Michelon

$Id: NetUDP.hpp,v 1.2 2002/11/29 14:26:06 hmichelon Exp $
---------------------------------------------------------------*/
#ifndef _NMO_NET_UDPBSDSOCK_H_
#define _NMO_NET_UDPBSDSOCK_H_

#include "NetIDIP.hpp"

namespace NMO {

class NetUDP: public NetMessage
{
public:
	NetUDP(const NetIDIP*, FileCfg&);
	virtual ~NetUDP();

	virtual const NetID& HostID();
	virtual _DWORD Send(const NetID*, const _PTR, _DWORD);
	virtual _DWORD Receive(const _PTR, _DWORD, NetID*&, _DWORD=0);

protected:
	NetIDIP		*netID;
};

inline const NetID& NetUDP::HostID()
{ return (NetID&)*netID; };

}

#endif
