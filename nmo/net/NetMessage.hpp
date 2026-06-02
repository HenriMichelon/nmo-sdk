/*                                ---------
                                  [NMO-SDK]
                                  ---------
*/
#ifndef _NMO_NET_MESSAGE_H_
#define _NMO_NET_MESSAGE_H_

namespace NMO {

/*!
\brief		Message based network communication procotol
			
			Protocol independant communication protocol for
			unconnected, message based, communications

			Only UDP/IP is implemented, but you are welcome
			to implement your prefered protocol.

\author		(c) 1999-2000 Henri Michelon
\version	$Id: NetMessage.hpp,v 1.1.1.1 2002/11/03 19:18:16 hmichelon Exp $*/
//--------------------------------------------------------------------------

class NetMessage: public NetProtocol
{
public:
	/*! Create a network protocol object
		\return NULL on error
	*/
	static NetMessage* Create(NetID*);
};

}

#endif
